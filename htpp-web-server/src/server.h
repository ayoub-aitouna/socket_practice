#ifndef SERVER_H
#define SERVER_H

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
#include <time.h>
#include <stdlib.h>
#include "linked_list/linked_list.h"
#include <poll.h>
#include <fcntl.h>

#define MAX_REQ_SIZE 2014

typedef struct  client_info
{
    int socket_fd;
    struct sockaddr_storage address;
    socklen_t address_len;
    char request[MAX_REQ_SIZE + 1];
    int recieved_bytes;
    struct  client_info *next;

} t_client_info;

#endif