#include "html.h"

dstring get_html_page(dstring filepath)
{
    // have the pages loaded on server startup (in a hashtable for example)
    // if there is no page with a route that is sent, return 404.html
    size_t len = get_file_size(filepath);
    dstring html = ds_new_string(len);
    assert(len == read_file(filepath, (void**)&html));
    return html;
}