#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    mqd_t mq;
    char buf[8192];

    // Подключаемся
    mq = mq_open("/my_queue", O_RDWR);

    // Читаем приветствие сервера
    ssize_t bytes = mq_receive(mq, buf, 8192, NULL);
    buf[bytes] = '\0';
    printf("Клиент Получил от сервера: %s\n", buf);

    // Сразу отвечаем
    mq_send(mq, "Hello!", 7, 0);
    printf("Клиент Отправил 'Hello!'\n");

    mq_close(mq);
    return 0;
}