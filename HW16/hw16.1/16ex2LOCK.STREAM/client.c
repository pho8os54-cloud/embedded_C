#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int fd;
    struct sockaddr_un serv;
    char buf[1024];

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "/tmp/stream_serv");
    connect(fd, (struct sockaddr *)&serv, sizeof(serv));

    send(fd, "hello!", 6, 0);

    int bytes = recv(fd, buf, 1024, 0);
    write(1, buf, bytes); write(1, "\n", 1);

    close(fd);
}