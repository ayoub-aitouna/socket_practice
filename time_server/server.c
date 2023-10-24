#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        return (printf("socket err "), 1);
    printf("created Socket ... \n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    
    addr.sin_addr.s_addr = INADDR_ANY;
    int bind_status = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (bind_status == -1)
        return (printf("bind err "), 1);
    printf("bound Socket ... \n");
    int listen_status = listen(socket_fd, 10);
    if (listen_status == -1)
        return (printf("listen err "), 1);
    printf("in listining mode ... \n");
    while (1)
    {
        struct sockaddr_storage client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1)
            return (printf("client connection err "), 1);
        printf("connection established  \n");
        char addrbuf[1024];
        getnameinfo((struct sockaddr *)&client_addr, client_len, addrbuf, sizeof(addrbuf), 0, 0, NI_NUMERICHOST);
        printf("%s\n", addrbuf);

        /*request */
        printf("reading request \n");
        char req[1024];
        int rb = recv(client_fd, req, sizeof(req), 0);
        printf("Recieved %d bytes \n", rb);
        printf("%.*s", rb, req);

        /*send data*/
        printf("send data \n");
        const char *res =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Time is : 11:11AM";
        rb = send(client_fd, res, strlen(res), 0);
        printf("Sent %d of %d bytes \n", rb, (int)strlen(res));
        close(client_fd);
    }
}