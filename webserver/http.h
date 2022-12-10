#ifndef HTTP_H
#define HTTP_H

#include <stdbool.h>

#include "../dynamic_string/dstring.h"
#include "server.h"

dstring get_method(dstring message);
dstring get_route(dstring message);

#endif