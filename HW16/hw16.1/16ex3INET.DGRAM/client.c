#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int fd;
    struct sockaddr_in serv;
    char buf[1024];

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);
    connect(fd, (struct sockaddr *)&serv, sizeof(serv));

    send(fd, "hello!", 6, 0);

    int bytes = recv(fd, buf, 1024, 0);
    write(1, buf, bytes); write(1, "\n", 1);

    close(fd);
}