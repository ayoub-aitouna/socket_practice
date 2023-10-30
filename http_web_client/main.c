#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>

typedef struct s_url_params
{
    char *protocol;
    char *hostname;
    char *port;
    char *path;

} t_url_params;

t_url_params input_parse(char *url)
{
    // http0//www.google.com/index.html?user=10
    t_url_params p_url;
    char *s = strstr(url, "://");
    if (s)
    {
        p_url.protocol = url;
        *s = 0;
        s += 3;
    }
    else
        s = url;
    p_url.hostname = s;
    while (*s && *s != '/' && *s != ':' && *s != '#')
        s++;
    p_url.port = "80";
    if (*s == ':')
    {
        *s++ = 0;
        p_url.port = s;
    }
    p_url.path = s;
    if (*s == '/')
        p_url.path = s + 1;
    *s = 0;
    while (*s && *s != '#')
        s++;
    if (*s == '#')
        *s = 0;
    return (p_url);
}

void send_request(int socket_fd, t_url_params url_params)
{
    char buffer[1024];
    sprintf(buffer, "GET /%s HTTP/1.1\r\n", url_params.path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", url_params.hostname, url_params.port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    send(socket_fd, buffer, strlen(buffer), 0);
    printf("%s", buffer);
}

int connect_to_host(t_url_params params)
{
    struct addrinfo hints;
    struct addrinfo *addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(params.hostname, params.port, &hints, &addr))
        return printf("getaddrinfo() failed \n"), -1;

    char addr_buffer[1024];
    char serv_buffer[1024];
    getnameinfo(addr->ai_addr, addr->ai_addrlen, addr_buffer, sizeof(addr_buffer),
                serv_buffer, sizeof(serv_buffer), NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s:%s \n", addr_buffer, serv_buffer);
    int socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (socket_fd < 0)
        return printf("socket() failed \n"), -1;

    if (connect(socket_fd, addr->ai_addr, addr->ai_addrlen) < 0)
        return printf("connect() failed \n"), -1;
    printf("Connected \n");
    freeaddrinfo(addr);
    return (socket_fd);
}

#define RESPONSE_SIZE 8192
#define TIMOUT 5
int main()
{
    char responce[RESPONSE_SIZE + 1];
    char *p = responce, *q;
    char *end = responce + RESPONSE_SIZE;
    char *body;

    enum {lenght, chuncked, connection};
    int encoding = 0;
    int remaining = 0;
    char url[] = "http://www.google.com/index.html?user=10";
    t_url_params p_url = input_parse(url);
    int socket_fd = connect_to_host(p_url);
    const clock_t start_time = clock();
    send_request(socket_fd, p_url);
}
