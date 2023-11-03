#include "server.h"

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
    if (!content_type)
        code = 404;
    message = get_status_message(codes, code);

    sprintf(buffer, "HTTP/1.1 %d %s\r\n", code, message);
    send(fd, buffer, strlen(buffer), 0);

    sprintf(buffer, "Connection: close\r\n");
    send(fd, buffer, strlen(buffer), 0);

    sprintf(buffer, "Content-Lenght: %ld\r\n", content_lenght);
    send(fd, buffer, strlen(buffer), 0);

    if (content_type)
    {
        sprintf(buffer, "Content-Type: %s\r\n", content_type);
        send(fd, buffer, strlen(buffer), 0);
    }

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
    if (strcmp(path, "/") == 0)
        path = "/index.html";
    if (path[strlen(path) - 1] == '/')
        sprintf(path, "%sindex.html", path);
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
        printf("coudn't open %s \n", full_path);
        fflush(stdout);
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
