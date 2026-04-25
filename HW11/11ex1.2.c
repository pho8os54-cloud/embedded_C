#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Структура магазина
struct shop {
    int kiosk1;
    int kiosk2;
    int kiosk3;
    int kiosk4;
    int kiosk5;
};

struct shop my_shop;
pthread_mutex_t m[5]; // Мьютекс на каждый ларек
int buyers_active = 3; // Счетчик активных покупателей

// Функция для получения указателя на нужный ларек по индексу
int* get_kiosk_ptr(int index) {
    switch (index) {
        case 0: return &my_shop.kiosk1;
        case 1: return &my_shop.kiosk2;
        case 2: return &my_shop.kiosk3;
        case 3: return &my_shop.kiosk4;
        case 4: return &my_shop.kiosk5;
        default: return NULL;
    }
}

// Поток покупателя
void* buyer(void* arg) {
    int id = *((int*)arg); // Получаем номер покупателя из переданного аргумента
    free(arg);
    
    int need = rand() % 201 + 9900; // Потребность 9900 - 10100
    printf("Покупатель %d родился, потребность = %d\n", id, need);

    while (need > 0) {
        int k = rand() % 5; // Выбираем киоск случайно
        int* kiosk_ptr = get_kiosk_ptr(k);// получаем адрес выбранного киоска

        printf("Покупатель %d проснулся, текущая потребность = %d\n", id, need);

        // заход в киоск
        pthread_mutex_lock(&m[k]);
        printf("Покупатель %d зашел в киоск %d, товаров там: %d\n", id, k + 1, *kiosk_ptr);

        int buy = *kiosk_ptr; //забираем весь товар
        if (buy > need) buy = need; // Не берем больше, чем нужно

        *kiosk_ptr -= buy; // уменьшение кол-ва товаров в киоске
        need -= buy; //уменьшение потребности

        printf("Покупатель %d купил %d ед., осталось потребности: %d\n", id, buy, need);
        printf("Покупатель %d вышел из киоска %d\n", id, k + 1);
        pthread_mutex_unlock(&m[k]);

        if (need > 0) //если потребность еще есть, то усыпляем покупателя
        {
            printf("Покупатель %d уснул на 2 секунды\n\n", id);
            sleep(2);
        }
    }

    printf("--- Покупатель %d полностью утолил потребность ---\n\n", id);
    buyers_active--; // Уменьшаем счетчик живых покупателей
    return NULL;
}

// Поток погрузчика
void* loader(void* arg) {
    while (buyers_active > 0) // грузчик работает, пока есть хотя бы 1 покупатель
    {
        int k = rand() % 5; //случ. киоск для пополнения
        int* kiosk_ptr = get_kiosk_ptr(k); //получение адреса

        printf("Погрузчик проснулся\n");

        pthread_mutex_lock(&m[k]);
        *kiosk_ptr += 200;
        printf("Погрузчик пополнил киоск %d на 200 ед. (стало: %d)\n", k + 1, *kiosk_ptr);
        pthread_mutex_unlock(&m[k]);

        printf("Погрузчик уснул на 1 секунду\n\n");
        sleep(1);
    }
    printf("Погрузчик закончил работу, покупателей больше нет.\n\n");
    return NULL;
}

int main() {
    srand(getpid());

    // Инициализация киосков и мьютексов
    my_shop.kiosk1 = rand() % 201 + 900;
    my_shop.kiosk2 = rand() % 201 + 900;
    my_shop.kiosk3 = rand() % 201 + 900;
    my_shop.kiosk4 = rand() % 201 + 900;
    my_shop.kiosk5 = rand() % 201 + 900;

    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&m[i], NULL);
    }

    printf("Стартовые запасы: %d, %d, %d, %d, %d\n\n", 
           my_shop.kiosk1, my_shop.kiosk2, my_shop.kiosk3, my_shop.kiosk4, my_shop.kiosk5);

    pthread_t b_threads[3], l_thread; // дескрипторы 4 потоков

    // Создаем погрузчика
    pthread_create(&l_thread, NULL, loader, NULL);

    // Создаем покупателей
    for (int i = 0; i < 3; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&b_threads[i], NULL, buyer, id);
    }

    // Ждем завершения покупателей
    for (int i = 0; i < 3; i++) {
        pthread_join(b_threads[i], NULL);
    }

    // Ждем завершения погрузчика
    pthread_join(l_thread, NULL);

    // Очистка
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&m[i]);
    }

    printf("Программа завершена.\n");
    return 0;
}