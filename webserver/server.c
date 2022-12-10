#include "server.h"

static void on_connection(client c);
static void on_connection_close(client c);
static void on_server_up(server s);

server new_server(int port)
{
    server serv;
    serv.socket = -1;
    int sock;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("couln't create socket.\n");
        exit(EXIT_FAILURE);
    }
    serv.socket = sock;

    // socket name and config
    serv.server_name.sin_family = AF_INET;
    serv.server_name.sin_port = htons(port);
    serv.server_name.sin_addr.s_addr = htonl(INADDR_ANY);

    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("couldn't set socket options.\n");
        exit(EXIT_FAILURE);
    }


    if(bind(sock, (struct sockaddr *)&serv.server_name, sizeof(serv.server_name)) < 0)
    {
        perror("couldn't bind name to socket.\n");
        exit(EXIT_FAILURE);
    }

    return serv;
}

void activate_server(server serv, int max_queue, bool status_messages)
{
    if (listen(serv.socket, max_queue) < 0)
    {
        perror("couldn't listen for connections.\n");
        exit(EXIT_FAILURE);
    }
    if (status_messages)
    {
        on_server_up(serv);
    }
}


client accept_connection(server serv, bool status_messages)
{
    client cli;

    int size = sizeof(struct sockaddr_in);
    cli.socket = accept(serv.socket, (struct sockaddr *)&(cli.client_name), (socklen_t *)&size);
    if (cli.socket < 0)
    {
        perror("couldn't accept connection.\n");
        exit(EXIT_FAILURE);
    }
    if (status_messages)
    {
        on_connection(cli);
    }
    return cli;
}

void close_connection(client* c, bool status_messages)
{
    close(c->socket);
    if (status_messages)
    {
        on_connection_close(*c);
    }
    memset(&c->client_name, 0, sizeof(struct sockaddr_in));
    c->socket = -1;
}

dstring read_message(client c, bool status_message)
{
   dstring message = ds_new_string(MESSAGE_CHUNK);
   read(c.socket, (void*)message, MESSAGE_CHUNK);
   return message;
}

int send_message(client c, dstring message, bool status_message)
{
    return write(c.socket, (void*)message, strlen(message));
}

// Status messages:

static void on_connection(client c)
{
    printf("Accepted connection from client %s, port %d\n", inet_ntoa(c.client_name.sin_addr), ntohs(c.client_name.sin_port));
}

static void on_connection_close(client c)
{
    printf("Closed connection with %s.\n\n\n", inet_ntoa(c.client_name.sin_addr));
}

static void on_server_up(server s)
{
    printf("Server %s listening to port %d.\n", inet_ntoa(s.server_name.sin_addr), ntohs(s.server_name.sin_port));
}