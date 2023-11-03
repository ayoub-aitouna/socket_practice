#include "server.h"

char *get_content_type(t_list *list, const char *path)
{
    char *key = strrchr(path, '.');
    while (list)
    {
        if (strcmp(key, list->key) == 0)
            return (list->value);
        list = list->next;
    }
    return ("application/octet-stream");
}

void init_meme_types(t_list **list)
{
    // text
    ft_lstadd_back(list, ft_lstnew(".text", "text/plain"));
    ft_lstadd_back(list, ft_lstnew(".css", "text/css"));
    ft_lstadd_back(list, ft_lstnew(".csv", "text/csv"));
    ft_lstadd_back(list, ft_lstnew(".htm", "text/html"));
    ft_lstadd_back(list, ft_lstnew(".html", "text/html"));

    // images
    ft_lstadd_back(list, ft_lstnew(".gif", "image/gif"));
    ft_lstadd_back(list, ft_lstnew(".ico", "image/x-icon"));
    ft_lstadd_back(list, ft_lstnew(".jpeg", "image/jpeg"));
    ft_lstadd_back(list, ft_lstnew(".jpg", "image/jpeg"));
    ft_lstadd_back(list, ft_lstnew(".png", "image/png"));
    ft_lstadd_back(list, ft_lstnew(".svg", "image/svg+xml"));

    // Application
    ft_lstadd_back(list, ft_lstnew(".js", "application/javascript"));
    ft_lstadd_back(list, ft_lstnew(".json", "application/json"));
    ft_lstadd_back(list, ft_lstnew(".pdf", "application/pdf"));
}

void init_status_codes(t_list **list)
{
    ft_lstadd_back(list, ft_lstnew("100", "Continue"));
    ft_lstadd_back(list, ft_lstnew("101", "Switching Protocols"));
    ft_lstadd_back(list, ft_lstnew("200", "OK"));
    ft_lstadd_back(list, ft_lstnew("201", "Created"));
    ft_lstadd_back(list, ft_lstnew("202", "Accepted"));
    ft_lstadd_back(list, ft_lstnew("203", "Non-Authoritative Information"));
    ft_lstadd_back(list, ft_lstnew("204", "No Content"));
    ft_lstadd_back(list, ft_lstnew("205", "Reset Content"));
    ft_lstadd_back(list, ft_lstnew("206", "Partial Content"));
    ft_lstadd_back(list, ft_lstnew("300", "Multiple Choices"));
    ft_lstadd_back(list, ft_lstnew("301", "Moved Permanently"));
    ft_lstadd_back(list, ft_lstnew("302", "Found"));
    ft_lstadd_back(list, ft_lstnew("303", "See Other"));
    ft_lstadd_back(list, ft_lstnew("304", "Not Modified"));
    ft_lstadd_back(list, ft_lstnew("305", "Use Proxy"));
    ft_lstadd_back(list, ft_lstnew("307", "Temporary Redirect"));
    ft_lstadd_back(list, ft_lstnew("400", "Bad Request"));
    ft_lstadd_back(list, ft_lstnew("401", "Unauthorized"));
    ft_lstadd_back(list, ft_lstnew("402", "Payment Required"));
    ft_lstadd_back(list, ft_lstnew("403", "Forbidden"));
    ft_lstadd_back(list, ft_lstnew("404", "Not Found"));
    ft_lstadd_back(list, ft_lstnew("405", "Method Not Allowed"));
    ft_lstadd_back(list, ft_lstnew("406", "Not Acceptable"));
    ft_lstadd_back(list, ft_lstnew("407", "Proxy Authentication Required"));
    ft_lstadd_back(list, ft_lstnew("408", "Request Timeout"));
    ft_lstadd_back(list, ft_lstnew("409", "Conflict"));
    ft_lstadd_back(list, ft_lstnew("410", "Gone"));
    ft_lstadd_back(list, ft_lstnew("411", "Length Required"));
    ft_lstadd_back(list, ft_lstnew("412", "Precondition Failed"));
    ft_lstadd_back(list, ft_lstnew("413", "Payload Too Large"));
    ft_lstadd_back(list, ft_lstnew("414", "URI Too Long"));
    ft_lstadd_back(list, ft_lstnew("415", "Unsupported Media Type"));
    ft_lstadd_back(list, ft_lstnew("416", "Range Not Satisfiable"));
    ft_lstadd_back(list, ft_lstnew("417", "Expectation Failed"));
    ft_lstadd_back(list, ft_lstnew("426", "Upgrade Required"));
    ft_lstadd_back(list, ft_lstnew("500", "Internal Server Error"));
    ft_lstadd_back(list, ft_lstnew("501", "Not Implemented"));
    ft_lstadd_back(list, ft_lstnew("502", "Bad Gateway"));
    ft_lstadd_back(list, ft_lstnew("503", "Service Unavailable"));
    ft_lstadd_back(list, ft_lstnew("504", "Gateway Timeout"));
    ft_lstadd_back(list, ft_lstnew("505", "HTTP Version Not Supported"));
}
int is_socket_open(int socket_fd)
{
    printf("checking %d if is open : ", socket_fd);
    int i = fcntl(socket_fd, F_GETFD);
    if (i < 0)
        return printf("not open \n"), perror("fcntl"), -1;
    return printf(" open \n"), 1;
}
char *get_status_message(t_list *list, int code)
{
    char key[60];
    sprintf(key, "%d", code);
    while (list)
    {
        if (strcmp(key, list->key) == 0)
            return (list->value);
        list = list->next;
    }
    return ("none");
}

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
    getnameinfo((struct sockaddr *)&client->address, client->address_len, buffer, 50, 0, 0, NI_NUMERICHOST);
    return (buffer);
}

struct pollfd *wait_on_client(t_client_info **clinets, int server_fd)
{
    struct pollfd *fds = malloc(10 * sizeof(struct pollfd));

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
    if (ready > 0)
        printf("ready  \n");
    return (fds);
}

void send_status(t_list *codes, t_client_info **clinets, t_client_info *client, int status_code)
{
    printf("sending status %d \n", status_code);
    char responce[1024];
    char *message = get_status_message(codes, status_code);
    sprintf(responce, "HTTP/1.1 %d %s \r\n"
                      "Connection: closed\r\n"
                      "Content-Length: %ld\r\n\r\n"
                      "%s",
            status_code, message, strlen(message), message);
    printf("resonse is :...\n\n%s\n\n...", responce);
    fflush(stdout);
    if (send(client->socket_fd, responce, strlen(responce), 0) < 0)
        perror("send() failed ");
    close(client->socket_fd);
    drop_client(clinets, client);
}

void send_headers(int fd, t_list *codes, int code, size_t content_lenght, char *content_type)
{
    char buffer[2028];
    char *message;
    message = get_status_message(codes, code);

    sprintf(buffer, "HTTP/1.1 %d %s\r\n", code, message);
    send(fd, buffer, strlen(buffer), 0);

    sprintf(buffer, "Connection: close\r\n");
    send(fd, buffer, strlen(buffer), 0);

    sprintf(buffer, "Content-Lenght: %ld\r\n", content_lenght);
    send(fd, buffer, strlen(buffer), 0);

    sprintf(buffer, "Content-Type: %s\r\n", content_type);
    send(fd, buffer, strlen(buffer), 0);

    send(fd, "\r\n", 2, 0);
}

void serve_resources(t_list *types, t_list *codes, t_client_info **clients, t_client_info *client, char *path)
{
    char full_path[128];
    size_t content_lenght;
    char *content_type;
    FILE *file;
    char buffer[100];

    printf("serving %s to client : %s \n", path, get_client_address(client));
    if (is_socket_open(client->socket_fd) < 0)
        return;
    if (strcmp(path, "/") == 0)
        path = "/index.html";
    if (!path || strlen(path) > 100)
        return send_status(codes, clients, client, 400);
    if (strstr(path, ".."))
    {
        printf("%s Not Found 404", path);
        return send_status(codes, clients, client, 404);
    }
    sprintf(full_path, "public%s", path);

    file = fopen(full_path, "rb");
    if (!file)
    {
        printf("file not found : %s\n", full_path);
        return send_status(codes, clients, client, 404);
    }
    fseek(file, 0L, SEEK_END);
    content_lenght = ftell(file);
    rewind(file);
    send_headers(client->socket_fd, codes, 200, content_lenght, get_content_type(types, full_path));

    while (1)
    {
        int r = fread(buffer, 1, 100, file);
        if (r == 0)
            break;
        send(client->socket_fd, buffer, r, 0);
    }
    printf("done serving file ... \n");
    drop_client(clients, client);
}

int main()
{
    // memes
    t_list *content_types = NULL;
    t_list *status_codes = NULL;
    t_client_info *clients = NULL;
    t_client_info *tmp;

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
                if (is_socket_open(tmp->socket_fd) < 0)
                {
                    tmp = next;
                    continue;
                }
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
                }
            }

            if (fds[i].revents && POLLOUT)
            {
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
            tmp = next;
            i++;
        }
    }
    printf("end executing \n");

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