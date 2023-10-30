#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *local_addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, "8080", &hints, &local_addr);

    printf("creating new Socket \n");
    int s_fd = socket(local_addr->ai_family, local_addr->ai_socktype, local_addr->ai_protocol);
    if (s_fd < 0)
        return (perror("socket() failed : "), 1);

    if (bind(s_fd, local_addr->ai_addr, local_addr->ai_addrlen) < 0)
        return (perror("bind() failed : "), 1);
    freeaddrinfo(local_addr);

    printf("listinig .... \n");
    if (listen(s_fd, 10) < 0)
        return (perror("listen() failed : "), 1);

    /**
     * Multiplexing Logic here
     */
    fd_set master;
    fd_set copy;

    FD_ZERO(&master);
    FD_SET(s_fd, &master);

    int max_socket = s_fd;
    while (1)
    {
        copy = master;
        if (select(max_socket + 1, &copy, 0, 0, 0) < 0)
            return (perror("select() failed : "), 1);

        for (int i = 1; i <= max_socket; i++)
        {
            if (FD_ISSET(i, &copy))
            {
                if (i == s_fd)
                {
                    printf("sf is ready \n");
                    int c_fd = accept(s_fd, NULL, NULL);
                    if (c_fd < 0)
                        return (perror("accept() failed : "), 1);
                    printf("accepted a new connection \n");
                    FD_SET(c_fd, &master);
                    if (c_fd > max_socket)
                        max_socket = c_fd;
                }
                else
                {
                    printf("socket : %d  is ready \n", i);
                    char buffer[4096];
                    int bytes = recv(i, buffer, sizeof(buffer), 0);
                    if (bytes < 0)
                    {
                        perror("recv() failed : ");
                        return (1);
                    }
                    printf("recived %d \n", bytes);
                    bytes = send(i, buffer, bytes, 0);
                    if (bytes < 0)
                    {
                        perror("send() failed : ");
                        return (1);
                    }
                }
            }
        }
    }
    return (0);
}