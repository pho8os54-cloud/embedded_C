#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int fd, size_cl;
    struct sockaddr_un serv, client;
    char buf[1024];

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    unlink("/tmp/dgram_serv");
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "/tmp/dgram_serv");
    bind(fd, (struct sockaddr *)&serv, sizeof(serv));

    size_cl = sizeof(client);
    int bytes = recvfrom(fd, buf, 1024, 0, (struct sockaddr *)&client, &size_cl);
    buf[bytes] = '\0'; // Выводим строку
    write(1, buf, bytes); write(1, "\n", 1);

    sendto(fd, "hi!", 3, 0, (struct sockaddr *)&client, size_cl);

    close(fd);
}