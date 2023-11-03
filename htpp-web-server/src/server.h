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
#include <signal.h>
#include <sys/epoll.h>


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
int create_socket(const char *host, const char *port);
t_client_info *get_client(t_client_info **clinets, int fd);
void drop_client(t_client_info **clinets, t_client_info *client);
char *get_client_address(t_client_info *client);
struct pollfd *wait_on_client(t_client_info **clinets, int server_fd);
char *get_status_message(t_list *list, int code);
void send_status(t_list *codes, t_client_info **clinets, t_client_info *client, int status_code);
void send_headers(int fd, t_list *codes, int code, size_t content_lenght, char *content_type);
void serve_resources(t_list *types, t_list *codes, t_client_info **clients, t_client_info *client, char *path);
char *get_content_type(t_list *list, const char *path);
void init_meme_types(t_list **list);
void init_status_codes(t_list **list);

#endif