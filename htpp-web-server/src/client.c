#include "server.h"

t_client_info *get_client(t_client_info **clinets, int fd)
{
    t_client_info *tmp;

    tmp = *clinets;
    while (tmp)
    {
        if (tmp->socket_fd == fd)
            return (tmp);
        tmp = tmp->next;
    }
    t_client_info *new = malloc(sizeof(t_client_info));
    memset(new, 0, sizeof(new));
    new->address_len = sizeof(new->address);
    new->next = *clinets;
    new->socket_fd = fd;
    new->recieved_bytes = 0;
    *clinets = new;
    return (new);
}

void drop_client(t_client_info **clinets, t_client_info *client)
{
    t_client_info **tmp;

    tmp = clinets;
    while (*tmp)
    {
        if (*tmp == client)
        {
            close(client->socket_fd);
            *tmp = client->next;
            free(client);
            return;
        }
        *tmp = (*tmp)->next;
    }
    printf("drop clinet not found \n");
    fflush(stdout);
    exit(1);
}

char *get_client_address(t_client_info *client)
{
    char *buffer = malloc(50);
    memset(buffer, 0, sizeof(buffer));
    if (!client)
        strcpy(buffer, "empty");
    else
        getnameinfo((struct sockaddr *)&client->address, client->address_len, buffer, 50, 0, 0, NI_NUMERICHOST);
    return (buffer);
}

struct pollfd *wait_on_client(t_client_info **clinets, int server_fd)
{
    struct pollfd *fds = malloc(50 * sizeof(struct pollfd));

    fds[0].fd = server_fd;
    fds[0].events = POLLIN | POLLOUT;

    int i = 1;
    t_client_info *tmp = *clinets;
    while (tmp)
    {
        fds[i].fd = tmp->socket_fd;
        fds[i].events = POLLIN | POLLOUT;
        tmp = tmp->next;
        i++;
    }
    int ready = poll(fds, i, 5000);
    if (ready < 0)
    {
        perror("poll() failed");
        exit(1);
    }
    return (fds);
}
