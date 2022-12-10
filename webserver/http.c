#include "http.h"

dstring get_method(dstring message)
{
    int i = ds_find_character(message, ' ');
    dstring method = ds_substring(message, 0, i);
    return method;
}

dstring get_route(dstring message)
{
    int i = ds_find_character(message, '/');
    int j = ds_find_character_start_at(message, i, ' ');
    dstring route = ds_substring(message, i, j-1);
    return route;
}