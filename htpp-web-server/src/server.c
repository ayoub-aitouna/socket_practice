
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

int main()
{
    t_list *list = NULL;
    init_meme_types(&list);
    printf("get_content_type > %s \n", (char *)get_content_type(list, "folder/home.css"));
}