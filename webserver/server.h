#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../dynamic_string/dstring.h"

#define MESSAGE_CHUNK 4096

typedef struct serv {
    int socket;
    struct sockaddr_in server_name;
}server;

typedef struct cli {
    int socket;
    struct sockaddr_in client_name;
}client;

server new_server(int port);
void activate_server(server serv, int max_queue, bool status_messages);
client accept_connection(server serv, bool status_messages);
void close_connection(client* c, bool status_messages);
dstring read_message(client c, bool status_message);
int send_message(client c, dstring message, bool status_message);

#endif