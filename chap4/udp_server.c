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
#include <netdb.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *local_addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, "8080", &hints, &local_addr);
    printf("creating a new Socket \n");
    int s = socket(local_addr->ai_family, local_addr->ai_socktype, local_addr->ai_protocol);
    if (s < 0)
        return (perror("socket() failed : "), 1);
    if (bind(s, local_addr->ai_addr, local_addr->ai_addrlen) < 0)
        return (perror("bind() failed : "), 1);

    /**
     * Multiplexing code here
     */
    fd_set master;
    fd_set copy;
    FD_ZERO(&master);
    FD_SET(s, &master);
    while (1)
    {
        FD_ZERO(&copy);
        copy = master;
        if (select(s + 1, &copy, 0, 0, 0) < 0)
            return (perror("select() failed : "), 1);
        if (FD_ISSET(s, &copy))
        {
            int bytes = recv();
        }
    }
}