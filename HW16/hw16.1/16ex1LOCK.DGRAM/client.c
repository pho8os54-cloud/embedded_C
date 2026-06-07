#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int fd;
    struct sockaddr_un serv, client;
    char buf[1024];

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    unlink("/tmp/dgram_cli");
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, "/tmp/dgram_cli");
    bind(fd, (struct sockaddr *)&client, sizeof(client));

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "/tmp/dgram_serv");
    connect(fd, (struct sockaddr *)&serv, sizeof(serv));

    send(fd, "hello!", 6, 0);

    int bytes = recv(fd, buf, 1024, 0);
    write(1, buf, bytes); write(1, "\n", 1);

    close(fd);
    unlink("/tmp/dgram_cli");
}