#include "server.h"

char *get_content_type(t_list *list, const char *path)
{
    char *key = strrchr(path, '.');
    if (!key)
        return NULL;
    while (list)
    {
        if (strcmp(key, list->key) == 0)
            return (list->value);
        list = list->next;
    }
    return ("application/octet-stream");
}

void init_meme_types(t_list **list)
{
    // text
    ft_lstadd_back(list, ft_lstnew(".text", "text/plain"));
    ft_lstadd_back(list, ft_lstnew(".css", "text/css"));
    ft_lstadd_back(list, ft_lstnew(".csv", "text/csv"));
    ft_lstadd_back(list, ft_lstnew(".htm", "text/html"));
    ft_lstadd_back(list, ft_lstnew(".html", "text/html"));

    // images
    ft_lstadd_back(list, ft_lstnew(".gif", "image/gif"));
    ft_lstadd_back(list, ft_lstnew(".ico", "image/x-icon"));
    ft_lstadd_back(list, ft_lstnew(".jpeg", "image/jpeg"));
    ft_lstadd_back(list, ft_lstnew(".jpg", "image/jpeg"));
    ft_lstadd_back(list, ft_lstnew(".png", "image/png"));
    ft_lstadd_back(list, ft_lstnew(".svg", "image/svg+xml"));

    // Application
    ft_lstadd_back(list, ft_lstnew(".js", "application/javascript"));
    ft_lstadd_back(list, ft_lstnew(".json", "application/json"));
    ft_lstadd_back(list, ft_lstnew(".pdf", "application/pdf"));
}

void init_status_codes(t_list **list)
{
    ft_lstadd_back(list, ft_lstnew("100", "Continue"));
    ft_lstadd_back(list, ft_lstnew("101", "Switching Protocols"));
    ft_lstadd_back(list, ft_lstnew("200", "OK"));
    ft_lstadd_back(list, ft_lstnew("201", "Created"));
    ft_lstadd_back(list, ft_lstnew("202", "Accepted"));
    ft_lstadd_back(list, ft_lstnew("203", "Non-Authoritative Information"));
    ft_lstadd_back(list, ft_lstnew("204", "No Content"));
    ft_lstadd_back(list, ft_lstnew("205", "Reset Content"));
    ft_lstadd_back(list, ft_lstnew("206", "Partial Content"));
    ft_lstadd_back(list, ft_lstnew("300", "Multiple Choices"));
    ft_lstadd_back(list, ft_lstnew("301", "Moved Permanently"));
    ft_lstadd_back(list, ft_lstnew("302", "Found"));
    ft_lstadd_back(list, ft_lstnew("303", "See Other"));
    ft_lstadd_back(list, ft_lstnew("304", "Not Modified"));
    ft_lstadd_back(list, ft_lstnew("305", "Use Proxy"));
    ft_lstadd_back(list, ft_lstnew("307", "Temporary Redirect"));
    ft_lstadd_back(list, ft_lstnew("400", "Bad Request"));
    ft_lstadd_back(list, ft_lstnew("401", "Unauthorized"));
    ft_lstadd_back(list, ft_lstnew("402", "Payment Required"));
    ft_lstadd_back(list, ft_lstnew("403", "Forbidden"));
    ft_lstadd_back(list, ft_lstnew("404", "Not Found"));
    ft_lstadd_back(list, ft_lstnew("405", "Method Not Allowed"));
    ft_lstadd_back(list, ft_lstnew("406", "Not Acceptable"));
    ft_lstadd_back(list, ft_lstnew("407", "Proxy Authentication Required"));
    ft_lstadd_back(list, ft_lstnew("408", "Request Timeout"));
    ft_lstadd_back(list, ft_lstnew("409", "Conflict"));
    ft_lstadd_back(list, ft_lstnew("410", "Gone"));
    ft_lstadd_back(list, ft_lstnew("411", "Length Required"));
    ft_lstadd_back(list, ft_lstnew("412", "Precondition Failed"));
    ft_lstadd_back(list, ft_lstnew("413", "Payload Too Large"));
    ft_lstadd_back(list, ft_lstnew("414", "URI Too Long"));
    ft_lstadd_back(list, ft_lstnew("415", "Unsupported Media Type"));
    ft_lstadd_back(list, ft_lstnew("416", "Range Not Satisfiable"));
    ft_lstadd_back(list, ft_lstnew("417", "Expectation Failed"));
    ft_lstadd_back(list, ft_lstnew("426", "Upgrade Required"));
    ft_lstadd_back(list, ft_lstnew("500", "Internal Server Error"));
    ft_lstadd_back(list, ft_lstnew("501", "Not Implemented"));
    ft_lstadd_back(list, ft_lstnew("502", "Bad Gateway"));
    ft_lstadd_back(list, ft_lstnew("503", "Service Unavailable"));
    ft_lstadd_back(list, ft_lstnew("504", "Gateway Timeout"));
    ft_lstadd_back(list, ft_lstnew("505", "HTTP Version Not Supported"));
}
