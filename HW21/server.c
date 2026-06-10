#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#define PORT_SERVER 39999
#define SIZEBUF 200
#define SIZE_H_UDP 8
#define SIZE_H_IP 20
#define MAX_CLIENTS 100

struct ClientInfo {
    struct in_addr ip;
    uint16_t port;
    int msg_count;
};

struct ClientInfo clients[MAX_CLIENTS];
int clients_count = 0;

int get_client_index(struct in_addr ip, uint16_t port) {
    for (int i = 0; i < clients_count; ++i) {
        if (clients[i].ip.s_addr == ip.s_addr && clients[i].port == port) {
            return i;
        }
    }
    return -1;
}

void remove_client(struct in_addr ip, uint16_t port) {
    int idx = get_client_index(ip, port);
    if (idx != -1) {
        for (int i = idx; i < clients_count - 1; ++i) {
            clients[i] = clients[i + 1];
        }
        clients_count--;
        printf("Client removed. Total clients: %d\n", clients_count);
    }
}

int main() {
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_SERVER);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return -1;
    }

    char recv_buf[SIZE_H_IP + SIZE_H_UDP + SIZEBUF];
    char send_buf[SIZE_H_UDP + SIZEBUF + 20];

    printf("Start raw server on port %d\n", PORT_SERVER);

    while (1) {
        struct sockaddr_in client_addr;
        int size = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));

        memset(recv_buf, 0, sizeof(recv_buf));
        if (recvfrom(fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&client_addr, &size) == -1) {
            perror("recvfrom");
            continue;
        }

        struct udphdr *recv_udp = (struct udphdr *)(recv_buf + SIZE_H_IP);

        // Проверяем, что пакет предназначен нашему серверу
        if (ntohs(recv_udp->dest) != PORT_SERVER) {
            continue;
        }

        // Извлекаем реальный порт клиента из UDP заголовка пакета
        uint16_t client_port = ntohs(recv_udp->source);
        
        // Принудительно записываем этот порт в структуру адреса клиента
        client_addr.sin_port = htons(client_port);

        // Если это наше собственное эхо, игнорируем его
        if (client_port == PORT_SERVER) {
            continue;
        }

        char *client_data = recv_buf + SIZE_H_IP + SIZE_H_UDP;

        // Обработка сигнала закрытия от клиента
        if (strcmp(client_data, "CLIENT_CLOSE_SIGNAL") == 0) {
            remove_client(client_addr.sin_addr, client_port);
            continue;
        }

        // Ищем клиента в нашей базе по связке IP:Порт
        int idx = get_client_index(client_addr.sin_addr, client_port);
        if (idx == -1) {
            if (clients_count < MAX_CLIENTS) {
                idx = clients_count;
                clients[idx].ip = client_addr.sin_addr;
                clients[idx].port = client_port;
                clients[idx].msg_count = 0;
                clients_count++;
                printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), client_port);
            } else {
                continue;
            }
        }

        clients[idx].msg_count++;
        printf("Client %s:%d, package count = %d\n", inet_ntoa(clients[idx].ip), clients[idx].port, clients[idx].msg_count);

        // Формируем текст ответа: "сообщение номер"
        char text_buf[SIZEBUF + 20];
        sprintf(text_buf, "%s %d", client_data, clients[idx].msg_count);

        struct udphdr *send_udp = (struct udphdr *)send_buf;
        char *send_data = send_buf + SIZE_H_UDP;
        strcpy(send_data, text_buf);

        int data_len = strlen(send_data) + 1;
        int udp_len = SIZE_H_UDP + data_len;

        // Собираем UDP заголовок для ответа
        send_udp->source = htons(PORT_SERVER);
        send_udp->dest = htons(client_port);
        send_udp->len = htons(udp_len);
        send_udp->check = 0;

        if (sendto(fd, send_buf, udp_len, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
            perror("sendto");
        }
    }

    close(fd);
    return 0;
}