#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#define PORT_SERVER 39999
#define SIZEBUF 200
#define SIZE_H_UDP 8

int global_fd = -1;
struct sockaddr_in global_server_addr;
uint16_t my_port = 0;

void replace_enter(char *str) {
    int i = 0;
    while ('\0' != str[i]) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        ++i;
    }
}

void handle_sigint(int sig) {
    (void)sig; 
    if (global_fd >= 0) {
        char send_buf[SIZE_H_UDP + SIZEBUF];
        struct udphdr *udp_packet = (struct udphdr *)send_buf;
        char *data_ptr = send_buf + SIZE_H_UDP;

        strcpy(data_ptr, "CLIENT_CLOSE_SIGNAL");
        int data_len = strlen(data_ptr) + 1;
        int udp_len = SIZE_H_UDP + data_len;

        udp_packet->source = htons(my_port);
        udp_packet->dest = htons(PORT_SERVER);
        udp_packet->len = htons(udp_len);
        udp_packet->check = 0;

        sendto(global_fd, send_buf, udp_len, 0, (struct sockaddr *)&global_server_addr, sizeof(global_server_addr));
        close(global_fd);
    }
    printf("\nExit via Ctrl+C. Signal sent to server.\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    // Инициализируем генератор случайных чисел уникальным значением времени и PID
    srand(time(NULL) ^ getpid());
    // Генерируем порт в диапазоне динамических портов: 49152 - 65535
    my_port = 49152 + (rand() % (65535 - 49152 + 1));
    printf("Client started on manual dynamic port: %d\n", my_port);

    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    global_fd = fd;

    memset(&global_server_addr, 0, sizeof(global_server_addr));
    global_server_addr.sin_family = AF_INET;
    global_server_addr.sin_port = htons(PORT_SERVER);
    inet_pton(AF_INET, "127.0.0.1", &global_server_addr.sin_addr);

    char send_buf[SIZE_H_UDP + SIZEBUF];
    char recv_buf[20 + SIZE_H_UDP + SIZEBUF];

    struct udphdr *udp_packet = (struct udphdr *)send_buf;
    char *data_ptr = send_buf + SIZE_H_UDP;

    while (1) {
        printf("Input: ");
        if (!fgets(data_ptr, SIZEBUF, stdin)) {
            continue;
        }
        replace_enter(data_ptr);

        int is_exit = (strcmp(data_ptr, "exit") == 0);
        
        if (is_exit) {
            strcpy(data_ptr, "CLIENT_CLOSE_SIGNAL");
        }

        int data_len = strlen(data_ptr) + 1;
        int udp_len = SIZE_H_UDP + data_len;

        udp_packet->source = htons(my_port);
        udp_packet->dest = htons(PORT_SERVER);
        udp_packet->len = htons(udp_len);
        udp_packet->check = 0;

        if (sendto(fd, send_buf, udp_len, 0, (struct sockaddr *)&global_server_addr, sizeof(global_server_addr)) == -1) {
            perror("sendto");
        }

        if (is_exit) {
            break;
        }

        while (1) {
            memset(recv_buf, 0, sizeof(recv_buf));
            struct sockaddr_in from_addr;
            int size = sizeof(from_addr);
            
            if (recvfrom(fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&from_addr, &size) == -1) {
                perror("recvfrom");
                break;
            }

            struct udphdr *recv_udp = (struct udphdr *)(recv_buf + 20);
            
            if (ntohs(recv_udp->source) == my_port && ntohs(recv_udp->dest) == PORT_SERVER) {
                continue; 
            }

            if (ntohs(recv_udp->source) == PORT_SERVER && ntohs(recv_udp->dest) == my_port) {
                char *recv_data = recv_buf + 20 + SIZE_H_UDP;
                printf("msg server: %s\n", recv_data);
                break;
            }
        }
    }

    close(fd);
    printf("End client\n");
    return 0;
}