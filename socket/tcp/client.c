#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../io/get_next_line/get_next_line.h"

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
    while (1)
    {
        printf("%s", get_next_line(socket_fd));
        char *msg = get_next_line(0);
        if (!msg)
            break;
        dprintf(socket_fd, "%s", msg);
    }
}