#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int fd, new_fd, len;
    struct sockaddr_un serv, client;
    char buf[1024];

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink("/tmp/stream_serv");
    
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "/tmp/stream_serv");
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