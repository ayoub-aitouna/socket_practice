#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        return (perror("socket : "), 1);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1)
        return (close(socket_fd), perror("connect : "), 1);
    char msg[] = "hello server is me clinet ";
    int r = write(socket_fd, msg, strlen(msg));
    if (r == -1)
        return (close(socket_fd), perror("write : "), 1);
}