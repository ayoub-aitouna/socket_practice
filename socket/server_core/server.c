#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../io/get_next_line/get_next_line.h"

void *handle_client(void *args)
{
    char *line;
    int client_fd = *(int *)args;
    while (1)
    {
        dprintf(client_fd, "Please Enter Message \n");
        line = get_next_line(client_fd);
        if (!line)
            break;
        printf("resived info \n{\n  %s}\n", line);
        fflush(stdout);
    }
    return NULL;
}

void accept_connections(int socket_fd)
{
    while (1)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientlen = sizeof(clientaddr);

        int client_fd = accept(socket_fd, (struct sockaddr *)&clientaddr, &clientlen);
        if (client_fd < 0)
            (perror("accept : "), exit(2));
        printf("a new connection has been established\n");
        pthread_t thread;
        int *fd = malloc(sizeof(int));
        *fd = client_fd;
        pthread_create(&thread, NULL, handle_client, fd);
    }
}

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
    accept_connections(socket_fd);

    close(socket_fd);
}
