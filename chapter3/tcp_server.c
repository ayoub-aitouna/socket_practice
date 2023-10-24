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
    hitn.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    geetaddrinfo(0, "8080", &hints, &local_addr);

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
    return (0);
}