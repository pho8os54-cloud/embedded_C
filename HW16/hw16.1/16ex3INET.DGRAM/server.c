#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int fd, size_cl;
    struct sockaddr_in serv, client;
    char buf[1024];

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8080);
    bind(fd, (struct sockaddr *)&serv, sizeof(serv));

    size_cl = sizeof(client);
    int bytes = recvfrom(fd, buf, 1024, 0, (struct sockaddr *)&client, &size_cl);
    write(1, buf, bytes); write(1, "\n", 1);

    sendto(fd, "hi!", 3, 0, (struct sockaddr *)&client, size_cl);

    close(fd);
}