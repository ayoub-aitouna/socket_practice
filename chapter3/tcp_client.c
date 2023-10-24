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

int main(int ac, char **av)
{
    if (ac < 3)
        return (printf("Invalide Arguments \n"), 1);

    struct addrinfo hints;
    struct addrinfo *addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(av[1], av[2], &hints, &addr);
    int socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (socket_fd < 0)
        return (printf("can't create a socket \n"), close(socket_fd), 1);

    printf("connecting ...\n");
    if (connect(socket_fd, addr->ai_addr, addr->ai_addrlen) < 0)
        return (printf("can't Connect to socket \n"), close(socket_fd), 1);
    printf("Connected \n");

    while (1)
    {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        int bytes;
        fd_set reads;

        FD_ZERO(&reads);

        FD_SET(socket_fd, &reads);
        FD_SET(0, &reads);

        struct timeval timout;
        timout.tv_sec = 0;
        timout.tv_usec = 100000;

        if (select(socket_fd + 1, &reads, 0, 0, &timout) < 0)
            return (printf("select() failed \n"), close(socket_fd), 1);
        if (FD_ISSET(socket_fd, &reads))
        {
            bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
            if (bytes <= 0)
                return (perror("recv() failed : "), close(socket_fd), 1);
            printf("recived data : \n%*s\n", bytes, buffer);
        }
        else if (FD_ISSET(0, &reads))
        {
            if (!fgets(buffer, sizeof(buffer), stdin))
                return (printf("fgets() failed \n"), 1);
            printf("sending {%s} ... \n", buffer);
            bytes = send(socket_fd, buffer, sizeof(buffer), 0);
            printf("sent %d bytes \n", bytes);
        }
    }
}
