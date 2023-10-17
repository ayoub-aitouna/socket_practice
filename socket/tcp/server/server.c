#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>

int main(int ac, char **av)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        return (perror("socket : "), 2);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int resure = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &resure, sizeof(int)) < 0)
        return (perror("resure : "), 2);

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return (perror("bind : "), 2);

    int listen_id = listen(socket_fd, 10);
    if (listen_id < 0)
        return (perror("listen : "), 2);

    while (1)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientlen = sizeof(clientaddr);

        int client_fd = accept(socket_fd, (struct sockaddr *)&clientaddr, &clientlen);
        if (client_fd < 0)
            return (perror("accept : "), 2);

        printf("accepted a new connection %d \n", client_fd);

        char buf[56];
        int recv_s;
        int ret = read(client_fd, buf, 56);
        if (ret == -1)
            return (printf("read : error"), 2);
        buf[ret] = 0;
        printf("resived info \n -- %s --\n", buf);
        fflush(stdout);
        // if (send(client_fd, buf, recv_s, 0) < 0)
        //     return (perror("send : "), 2);
        // close(client_fd);
    }
    close(socket_fd);
}
