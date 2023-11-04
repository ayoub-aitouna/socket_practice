#include "server.h"

int create_socket(const char *host, const char *port)
{
    struct addrinfo hint;
    struct addrinfo *addr;
    int socket_fd;

    printf("Configuring Local address \n");

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    getaddrinfo(host, port, &hint, &addr);

    printf("Creating Socket \n");
    int resure = 1;
    socket_fd = socket(addr->ai_family, addr->ai_socktype, 0);

    if (socket_fd < 0)
        return perror("socket() failed \n"), -1;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &resure, sizeof(int)) < 0)
        return (perror("resure : "), 2);

    if (bind(socket_fd, addr->ai_addr, addr->ai_addrlen) < 0)
        return perror("bind() failed \n"), -1;

    freeaddrinfo(addr);

    printf("Listinig \n");
    if (listen(socket_fd, 10) < 0)
        return perror("listen() failed \n"), -1;
    return (socket_fd);
}

void sigpipe_handler(int signum)
{
    (void)signum;
}

int main()
{
    // memes
    t_list *content_types = NULL;
    t_list *status_codes = NULL;
    t_client_info *clients = NULL;
    t_client_info *tmp;
    signal(SIGPIPE, sigpipe_handler);

    init_meme_types(&content_types);
    init_status_codes(&status_codes);

    int server_fd = create_socket(0, "8080");
    if (server_fd < 0)
        return printf("create_socket() failled \n"), 1;

    while (1)
    {
        struct pollfd *fds;
        fds = wait_on_client(&clients, server_fd);

        if (fds[0].revents && POLLIN)
        {
            t_client_info *new_client = get_client(&clients, -1);
            new_client->socket_fd = accept(server_fd,
                                           (struct sockaddr *)&new_client->address,
                                           &new_client->address_len);
            if (new_client->socket_fd < 0)
                return printf("err or while accepting new connection \n"),
                       1;
            printf("new connection from %s\n", get_client_address(new_client));
            continue;
        }

        tmp = clients;
        int i = 1;
        while (tmp)
        {
            t_client_info *next = tmp->next;
            if (fds[i].revents && POLLIN)
            {
                if (tmp->recieved_bytes == MAX_REQ_SIZE)
                    return send_status(status_codes, &clients, tmp, 400), 1;

                int rb = recv(tmp->socket_fd, tmp->request + tmp->recieved_bytes,
                              MAX_REQ_SIZE - tmp->recieved_bytes, 0);
                printf("recived %d bytes \n", rb);
                if (rb < 1)
                {
                    printf("connection has been closed by peer %s\n", get_client_address(tmp));
                    drop_client(&clients, tmp);
                }
                else
                {
                    tmp->recieved_bytes += rb;
                    tmp->request[tmp->recieved_bytes] = 0;
                    if (strstr(tmp->request, "\r\n\r\n"))
                    {
                        if (strncmp(tmp->request, "GET /", 5) == 0)
                        {
                            char *path = tmp->request + 4;
                            char *end = strstr(path, " ");
                            if (!end)
                                return send_status(status_codes, &clients, tmp, 400), 1;
                            *end = 0;
                            serve_resources(content_types, status_codes, &clients, tmp, path);
                        }
                        else
                            return send_status(status_codes, &clients, tmp, 40), 1;
                    }
                }
            }

            tmp = next;
            i++;
        }
    }
    tmp = clients;
    while (tmp)
    {
        t_client_info *next = tmp->next;
        free(tmp);
        tmp = next;
    }
    ft_lstclear(&content_types, free);
    ft_lstclear(&status_codes, free);
}