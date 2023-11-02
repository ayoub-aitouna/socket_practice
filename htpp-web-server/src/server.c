
#include "server.h"

const char *get_content_type(t_list *list, const char *path)
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
    socket_fd = socket(addr->ai_family, addr->ai_socktype, 0);
    if (socket_fd < 0)
        return printf("socket() failed \n"), -1;
    if (bind(socket_fd, addr->ai_addr, addr->ai_addrlen) < 0)
        return printf("bind() failed \n"), -1;
    freeaddrinfo(addr);

    printf("Listinig \n");
    if (listen(socket_fd, 10) < 0)
        return printf("listen() failed \n"), -1;
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
    exit(1);
}

char *get_client_address(t_client_info *client)
{
    char *buffer = malloc(100);
    getnameinfo((struct sockaddr *)&client->address, client->address_len, buffer, sizeof(buffer), 0, 0, NI_NUMERICHOST);
    return (buffer);
}

fd_set wait_on_client(t_client_info **clinets, int srver_fd)
{
    t_client_info *tmp;
    struct timeval time;
    fd_set reads;

    FD_ZERO(&reads);
    FD_SET(srver_fd, &reads);
    int max_socket = srver_fd;
    tmp = *clinets;
    time.tv_sec = 0;
    time.tv_usec = 2000;

    while (tmp)
    {
        FD_SET(tmp->socket_fd, &reads);
        if (max_socket < tmp->socket_fd)
            max_socket = tmp->socket_fd;
        tmp = tmp->next;
    }
    if (select(max_socket, &reads, 0, 0, &time) < 0)
    {
        printf("select() failed");
        exit(1);
    }
    return (reads);
}

void send_status(t_list *codes, t_client_info **clinets, t_client_info *client, int status_code)
{
    char responce[1024];
    char *message = get_status_message(codes, status_code);
    sprintf(responce, "HTTP/1.1 %d %s \r\n"
                      "Connection: closed\r\n"
                      "Content-Length: %ld\r\n\r\n"
                      "%s",
            status_code, message, strlen(message), message);
    send(client->socket_fd, responce, sizeof(responce), 0);
    drop_client(clinets, client);
}

void serve_resources(t_list *types, t_list *codes, t_client_info *clients, t_client_info *client, char *path)
{
    char full_path[128];
    printf("serving %s to client : %s \n", path, get_client_address(client));
    if (strcmp(path, "/") == 0)
        path = "/index.html";
    if (!path || strlen(path) > 100)
        return send_status(codes, clients, client, 400);
    if (strstr(path, ".."))
        return send_status(codes, clients, client, 404);
    sprintf(full_path, "public%s", path);

    FILE *file = fopen(full_path, "rb");
    if (!file)
        return send_status(codes, clients, client, 404);
    fseek(file, 0L, SEEK_END);
    size_t cl = ftell(file);
    rewind(file);

    char ct = get_content_type(types, full_path);
}

int main()
{
    // memes
    t_list *content_types = NULL;
    t_list *status_codes = NULL;
    init_meme_types(&content_types);
    init_status_codes(&status_codes);
    // clients
    t_client_info *clients = NULL;
    get_client(&clients, 5);

    send_status(status_codes, &clients, clients, 200);
}