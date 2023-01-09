#include "server.h"
#include "http.h"
#include "html.h"

int main()
{
    // test the program by opening the loop-back adress in the browser:
    // 127.0.0.1:6000/
    // Might have to add a port exception to firefox in about:config
    // (check the picture in this directory)
    //https://developer.mozilla.org/en-US/docs/Web/HTTP/Status 
    char buffer[4096];
    server s = new_server(6000);
    client c;
    c.socket = -1;
    activate_server(s, 1, true);
    dstring method;
    dstring route;
    while(1)
    {
        c = accept_connection(s, true);
        dstring recieved_message = read_message(c, true);
        //puts(recieved_message); // use this for debugging 
        method = get_method(recieved_message);
        route = get_route(recieved_message);
        printf("Recived: %s %s\n", method, route);
        if (strcmp(route, "/") == 0) // replace this with hashtable lookup (route string is key and value is filename e.g. route: "/" file: "index.html" or "/home" "home.html")
        {
            dstring html = get_html_page("test.html");
            dstring http = ds_new_string_initialize("HTTP/1.1 200 OK\n\n"); // these respones should be calculated in http.c with a response function or something 
            dstring result = ds_concat(http, html);
            puts("Responding:\n");
            puts(result);
            send_message(c, result, true);
            ds_delete(&html);
            ds_delete(&http);
            ds_delete(&result);
        }
        else if (strcmp(route, "/favicon.ico") == 0)
        {
            dstring http = ds_new_string_initialize("HTTP/1.1 204 No Content\n\n");
            puts("Responding:\n\n");
            puts(http);
            send_message(c, http, true);
            ds_delete(&http);
        }
        else
        {
            dstring http = ds_new_string_initialize("HTTP/1.1 400 Bad Request\n\n");
            puts("Responding:\n\n");
            puts(http);
            send_message(c, http, true);
            ds_delete(&http);
        }
        ds_delete(&method);
        ds_delete(&route);
        ds_delete(&recieved_message);
        if (c.socket > 0)
        {
            close_connection(&c, true);
        }
    }
}