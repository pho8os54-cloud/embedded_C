#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int fd, new_fd, len;
    struct sockaddr_in serv, client;
    char buf[1024];

    fd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8080);
    bind(fd, (struct sockaddr *)&serv, sizeof(serv));

    listen(fd, 5);

    len = sizeof(client);
    new_fd = accept(fd, (struct sockaddr *)&client, &len);

    int bytes = recv(new_fd, buf, 1024, 0);
    write(1, buf, bytes); write(1, "\n", 1);

    send(new_fd, "hi!", 3, 0);

    close(new_fd);
    close(fd);
}