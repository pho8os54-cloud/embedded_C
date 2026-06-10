#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <time.h>
#include <poll.h>
#include <errno.h>

// Максимальное количество водителей в системе
#define MAX_DRIVERS 50
// Имя главной очереди сообщений для CLI
#define CLI_QUEUE_NAME "/cli_queue_taxi"
// Размер буфера для сообщений
#define MSG_SIZE 128

// Перечисление для состояний водителя
typedef enum {
    STATE_AVAILABLE, // Водитель свободен
    STATE_BUSY // Водитель занят выполнением задачи
} driver_state_t;

// Структура для хранения информации о водителе в CLI
typedef struct {
    pid_t pid; // Идентификатор процесса водителя
    mqd_t mq_fd; // Дескриптор очереди сообщений этого водителя
    char q_name[32]; // Имя очереди сообщений водителя
    driver_state_t state; // Текущее состояние
    int remaining_time; // Оставшееся время работы в секундах
} driver_info_t;

// Массив для хранения данных всех запущенных водителей
driver_info_t drivers[MAX_DRIVERS];
// Текущее количество созданных водителей
int driver_count = 0;
// Дескриптор главной очереди CLI
mqd_t cli_mq;

// Глобальные переменные для дочернего процесса водителя
driver_state_t global_driver_state = STATE_AVAILABLE; // Начальный статус свободен
volatile int global_remaining_time = 0; // Оставшееся время задачи, изменяется в обработчике сигнала
timer_t driver_timer; // Идентификатор POSIX таймера
char driver_queue_name[32]; // Имя личной очереди этого водителя
mqd_t my_mq; // Дескриптор личной очереди этого водителя

// Обработчик сигналов от таймера, срабатывает раз в секунду
void handle_driver_timer(int sig, siginfo_t *si, void *uc) {
    // Если время задачи еще не истекло
    if (global_remaining_time > 0) {
        // Уменьшаем счетчик на одну секунду
        global_remaining_time--;
        // Если время вышло, переводим водителя в статус свободен
        if (global_remaining_time == 0) {
            global_driver_state = STATE_AVAILABLE;
        }
    }
}

// Настройка POSIX таймера и обработка сигналов реального времени
void setup_driver_timer() {
    struct sigaction sa;
    // Настраиваем использование расширенного обработчика с флагом SA_SIGINFO
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_driver_timer;
    // Очищаем маску сигналов
    sigemptyset(&sa.sa_mask);
    // Регистрируем обработчик для сигнала реального времени
    sigaction(SIGRTMIN, &sa, NULL);

    struct sigevent sev;
    // Задаем уведомление через отправку сигнала
    sev.sigev_notify = SIGEV_SIGNAL;
    // Указываем конкретный сигнал для отправки
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &driver_timer;
    // Создаем таймер, привязанный к системному времени
    timer_create(CLOCK_REALTIME, &sev, &driver_timer);
}

// Запуск таймера на заданное количество секунд
void start_driver_timer(int seconds) {
    // Записываем время работы
    global_remaining_time = seconds;
    // Меняем локальный статус на занят
    global_driver_state = STATE_BUSY;

    struct itimerspec its;
    // Первый запуск произойдет через 1 секунду
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    // Таймер будет повторяться каждую 1 секунду
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    // Активируем таймер с подготовленными параметрами
    timer_settime(driver_timer, 0, &its, NULL);
}

// Остановка таймера водителя
void stop_driver_timer() {
    struct itimerspec its;
    // Зануляем структуру, чтобы сбросить таймер
    memset(&its, 0, sizeof(its));
    // Передаем нулевые параметры для выключения таймера
    timer_settime(driver_timer, 0, &its, NULL);
}

// Основной цикл работы процесса водителя
void driver_process_loop(pid_t pid) {
    // Формируем уникальное имя очереди на основе PID процесса
    snprintf(driver_queue_name, sizeof(driver_queue_name), "/driver_q_%d", pid);
    
    struct mq_attr attr;
    attr.mq_flags = 0; // Флаги блокировки отсутствуют
    attr.mq_maxmsg = 10; // Максимальное количество сообщений в очереди
    attr.mq_msgsize = MSG_SIZE; // Максимальный размер одного сообщения
    attr.mq_curmsgs = 0; // Начальное количество сообщений равно нулю

    // Удаляем старую очередь с таким именем, если она осталась в системе
    mq_unlink(driver_queue_name);
    // Создаем новую очередь только для чтения
    my_mq = mq_open(driver_queue_name, O_CREAT | O_RDONLY, 0644, &attr);
    // Если создать не удалось, завершаем процесс водителя с ошибкой
    if (my_mq == (mqd_t)-1) {
        exit(1);
    }

    // Открываем главную очередь CLI только для записи ответов
    mqd_t cli_mq_fd = mq_open(CLI_QUEUE_NAME, O_WRONLY);
    // Инициализируем таймер для текущего водителя
    setup_driver_timer();

    char buffer[MSG_SIZE];

    // Бесконечный цикл ожидания команд от CLI
    while (1) {
        // Читаем команду из своей очереди, блокируемся до поступления данных
        ssize_t bytes_read = mq_receive(my_mq, buffer, MSG_SIZE, NULL);
        if (bytes_read > 0) {
            // Добавляем символ конца строки в буфер
            buffer[bytes_read] = '\0';

            char cmd[32];
            int val = 0;
            // Разбираем строку на команду и числовое значение
            sscanf(buffer, "%s %d", cmd, &val);

            char response[MSG_SIZE];
            // Обработка команды назначения задачи
            if (strcmp(cmd, "task") == 0) {
                // Если водитель уже занят, формируем сообщение об ошибке с остатком времени
                if (global_driver_state == STATE_BUSY) {
                    snprintf(response, sizeof(response), "RESP %d BUSY %d", pid, global_remaining_time);
                } else {
                    // Если свободен, запускаем таймер на указанное число секунд
                    start_driver_timer(val);
                    // Формируем успешный ответ
                    snprintf(response, sizeof(response), "RESP %d OK", pid);
                }
                // Отправляем ответ в главную очередь CLI
                mq_send(cli_mq_fd, response, strlen(response), 0);
            } 
            // Обработка запроса статуса
            else if (strcmp(cmd, "status") == 0) {
                // Если занят, отправляем статус BUSY и оставшееся время
                if (global_driver_state == STATE_BUSY) {
                    snprintf(response, sizeof(response), "RESP %d STATUS BUSY %d", pid, global_remaining_time);
                } else {
                    // Если свободен, отправляем статус AVAILABLE
                    snprintf(response, sizeof(response), "RESP %d STATUS AVAILABLE", pid);
                }
                // Отправляем сформированный ответ в CLI
                mq_send(cli_mq_fd, response, strlen(response), 0);
            }
        }
    }

    // Закрываем дескрипторы и удаляем очередь при выходе, хотя цикл бесконечный
    mq_close(my_mq);
    mq_close(cli_mq_fd);
    mq_unlink(driver_queue_name);
    exit(0);
}

// Создание нового процесса водителя из CLI
void create_driver() {
    // Проверяем, не превышен ли лимит созданных водителей
    if (driver_count >= MAX_DRIVERS) {
        printf("Максимальное количество водителей достигнуто.\n");
        return;
    }

    // Создаем дочерний процесс
    pid_t pid = fork();

    // Если fork вернул отрицательное значение, произошла ошибка
    if (pid < 0) {
        perror("Ошибка fork");
        return;
    }

    // Ветка дочернего процесса
    if (pid == 0) {
        // Запускаем основной цикл водителя и передаем его PID
        driver_process_loop(getpid());
    } else {
        // Ветка родительского процесса CLI
        driver_info_t d;
        d.pid = pid; // Сохраняем PID нового водителя
        d.state = STATE_AVAILABLE; // Начальный статус в таблице CLI свободен
        d.remaining_time = 0; // Время изначально равно нулю
        // Генерируем имя очереди, которую должен создать дочерний процесс
        snprintf(d.q_name, sizeof(d.q_name), "/driver_q_%d", pid);
        
        int retries = 0;
        // Пытаемся открыть очередь водителя на запись
        d.mq_fd = mq_open(d.q_name, O_WRONLY);
        
        // Если очередь еще не создана дочерним процессом, ждем и повторяем попытки
        while (d.mq_fd == (mqd_t)-1 && retries < 50) {
            // Засыпаем на 20 миллисекунд для устранения гонки процессов
            usleep(20000);
            // Пробуем открыть очередь снова
            d.mq_fd = mq_open(d.q_name, O_WRONLY);
            retries++;
        }

        // Если после всех попыток очередь не открылась, выводим ошибку
        if (d.mq_fd == (mqd_t)-1) {
            perror("Ошибка открытия очереди водителя в CLI");
            return;
        }

        // Добавляем заполненную структуру в массив водителей и увеличиваем счетчик
        drivers[driver_count++] = d;
        printf("Создан водитель с PID: %d\n", pid);
    }
}

// Поиск индекса водителя в массиве по его PID
int find_driver_index(pid_t pid) {
    for (int i = 0; i < driver_count; i++) {
        // Если PID совпал, возвращаем текущий индекс массива
        if (drivers[i].pid == pid) {
            return i;
        }
    }
    // Если водитель с таким PID не найден, возвращаем минус один
    return -1;
}

// Обработка входящих сообщений от водителей в очереди CLI
void handle_cli_messages() {
    char buffer[MSG_SIZE];
    // Читаем сообщение из главной очереди CLI без блокировки, так как poll подтвердил наличие данных
    ssize_t bytes_read = mq_receive(cli_mq, buffer, MSG_SIZE, NULL);
    if (bytes_read > 0) {
        // Устанавливаем символ конца строки
        buffer[bytes_read] = '\0';
        
        char tag[16];
        pid_t pid;
        char status_str[32];
        int time_val = 0;

        // Разбираем строку ответа на статус BUSY с указанием времени
        if (sscanf(buffer, "%s %d STATUS %s %d", tag, &pid, status_str, &time_val) == 4) {
            int idx = find_driver_index(pid);
            if (idx != -1) {
                if (strcmp(status_str, "BUSY") == 0) {
                    // Обновляем состояние водителя в локальной таблице CLI
                    drivers[idx].state = STATE_BUSY;
                    drivers[idx].remaining_time = time_val;
                    printf("\nСтатус Водитель %d: Busy %d сек.\n", pid, time_val);
                }
            }
        } 
        // Разбираем строку ответа на статус AVAILABLE
        else if (sscanf(buffer, "%s %d STATUS %s", tag, &pid, status_str) == 3) {
            int idx = find_driver_index(pid);
            if (idx != -1) {
                if (strcmp(status_str, "AVAILABLE") == 0) {
                    // Фиксируем в таблице CLI, что водитель освободился
                    drivers[idx].state = STATE_AVAILABLE;
                    drivers[idx].remaining_time = 0;
                    printf("\nСтатус Водитель %d: Available\n", pid);
                }
            }
        }
        // Разбираем отказ водителя выполнять новую задачу, так как он занят
        else if (sscanf(buffer, "%s %d BUSY %d", tag, &pid, &time_val) == 3) {
            int idx = find_driver_index(pid);
            if (idx != -1) {
                drivers[idx].state = STATE_BUSY;
                drivers[idx].remaining_time = time_val;
            }
            printf("\nОшибка: Водитель %d занят. Busy %d\n", pid, time_val);
        }
        // Разбираем успешное подтверждение приема задачи водителем
        else if (sscanf(buffer, "%s %d OK", tag, &pid) == 2) {
            int idx = find_driver_index(pid);
            if (idx != -1) {
                // Отмечаем водителя как занятого в таблице CLI
                drivers[idx].state = STATE_BUSY;
            }
            printf("\nЗадача для водителя %d успешно запущена.\n", pid);
        }
    }
}

// Обработка текстовых команд, введенных пользователем в терминале
void process_cli_command(char *cmd_line) {
    char cmd[32];
    pid_t pid = 0;
    int timer_val = 0;

    // Извлекаем название команды и возможные аргументы
    int tokens = sscanf(cmd_line, "%s %d %d", cmd, &pid, &timer_val);
    // Если строка пустая, выходим из функции
    if (tokens < 1) return;

    // Команда создания нового водителя
    if (strcmp(cmd, "create_driver") == 0) {
        create_driver();
    } 
    // Команда отправки задачи водителю
    else if (strcmp(cmd, "send_task") == 0) {
        // Проверяем наличие всех обязательных параметров
        if (tokens < 3) {
            printf("Использование: send_task pid task_timer\n");
            return;
        }
        int idx = find_driver_index(pid);
        // Если в таблице CLI нет такого PID, прерываем выполнение
        if (idx == -1) {
            printf("Водитель с PID %d не найден.\n", pid);
            return;
        }
        char msg[MSG_SIZE];
        // Формируем сообщение с параметром времени задачи
        snprintf(msg, sizeof(msg), "task %d", timer_val);
        // Отправляем сообщение в персональную очередь выбранного водителя
        mq_send(drivers[idx].mq_fd, msg, strlen(msg), 0);
    } 
    // Команда запроса статуса у конкретного водителя
    else if (strcmp(cmd, "get_status") == 0) {
        if (tokens < 2) {
            printf("Использование: get_status pid\n");
            return;
        }
        int idx = find_driver_index(pid);
        if (idx == -1) {
            printf("Водитель с PID %d не найден.\n", pid);
            return;
        }
        char msg[MSG_SIZE];
        // Формируем текстовый запрос статуса
        snprintf(msg, sizeof(msg), "status");
        // Направляем запрос в очередь водителя
        mq_send(drivers[idx].mq_fd, msg, strlen(msg), 0);
    } 
    // Команда вывода списка всех водителей из локальной таблицы CLI
    else if (strcmp(cmd, "get_drivers") == 0) {
        printf("\n=== Список водителей ===\n");
        for (int i = 0; i < driver_count; i++) {
            if (drivers[i].state == STATE_BUSY) {
                printf("PID: %d | Статус: Busy\n", drivers[i].pid);
            } else {
                printf("PID: %d | Статус: Available\n", drivers[i].pid);
            }
        }
        printf("========================\n");
    } 
    // Если введенная команда не совпала ни с одной из списка
    else {
        printf("Неизвестная команда.\n");
    }
}

// Главная функция приложения CLI
int main() {
    struct mq_attr attr;
    attr.mq_flags = 0; // Режим блокировки по умолчанию
    attr.mq_maxmsg = 10; // Вместимость очереди CLI
    attr.mq_msgsize = MSG_SIZE; // Максимальная длина строки сообщения
    attr.mq_curmsgs = 0; // Изначально сообщений нет

    // Удаляем старую главную очередь CLI, если программа завершилась некорректно в прошлый раз
    mq_unlink(CLI_QUEUE_NAME);
    // Создаем главную очередь CLI только для чтения входящих ответов от процессов водителей
    cli_mq = mq_open(CLI_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    // Если открыть не удалось, завершаем работу главной программы
    if (cli_mq == (mqd_t)-1) {
        perror("Ошибка создания главной очереди CLI");
        return 1;
    }

    struct pollfd fds[2];
    // Первый дескриптор для отслеживания ввода из терминала
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN; // Интересует событие появления входных данных
    // Второй дескриптор для отслеживания сообщений в главной очереди CLI
    fds[1].fd = cli_mq;
    fds[1].events = POLLIN; // Интересует событие прихода нового сообщения

    char input_buf[256];

    printf("Приложение управления такси запущено.\n");
    printf("Доступные команды: create_driver, send_task, get_status, get_drivers\n");

    // Главный цикл мультиплексирования ввода-вывода
    while (1) {
        printf("taxi_cli> ");
        fflush(stdout);

        // Блокируем выполнение и ждем событий на любом из дескрипторов
        int ret = poll(fds, 2, -1);
        if (ret < 0) {
            // Если poll прерван системным сигналом, просто игнорируем и продолжаем цикл
            if (errno == EINTR) continue;
            perror("Ошибка poll");
            break;
        }

        // Проверяем, появились ли данные в стандартном вводе терминала
        if (fds[0].revents & POLLIN) {
            // Считываем строку, введенную пользователем
            if (fgets(input_buf, sizeof(input_buf), stdin) != NULL) {
                // Передаем строку на разбор и выполнение команд
                process_cli_command(input_buf);
            }
        }

        // Проверяем, пришли ли сообщения от водителей в очередь CLI
        if (fds[1].revents & POLLIN) {
            // Вызываем обработку ответов из очереди
            handle_cli_messages();
        }
    }

    // Перед завершением работы закрываем все дескрипторы очередей и уничтожаем дочерние процессы
    for (int i = 0; i < driver_count; i++) {
        mq_close(drivers[i].mq_fd);
        kill(drivers[i].pid, SIGKILL); // Отправляем сигнал уничтожения процессу водителя
    }
    // Закрываем и удаляем главную очередь CLI из операционной системы
    mq_close(cli_mq);
    mq_unlink(CLI_QUEUE_NAME);

    return 0;
}