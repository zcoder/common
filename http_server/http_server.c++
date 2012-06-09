/* 
 * File:   http_server.c++
 * Author: zhenka
 * 
 * Created on 9 Июнь 2012 г., 6:07
 */

#include "http_server.h"

http_server::http_server( const in_addr_t in_addr, const in_port_t in_port, const uint32_t in_thread_count )
{
    struct sockaddr_in l_sockaddr_in;
    memset(&l_sockaddr_in, 0, sizeof(l_sockaddr_in));
    l_sockaddr_in.sin_family=AF_INET;
    l_sockaddr_in.sin_addr.s_addr=in_addr;
    l_sockaddr_in.sin_port=in_port;
    int m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) 
        throw std::exception();
    http_server(m_socket);
}

http_server::http_server( int socket, const uint32_t in_thread_count )
{
    m_socket = socket;
    int value = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(int));
    int result = bind(m_socket, (struct sockaddr*)&m_sockaddr_in, sizeof(m_sockaddr_in));
    if (result < 0) 
        throw std::exception;
    result = listen(m_socket, 10240);
    if (result < 0)
        throw std::exception;
    int flags;
    if ((flags = fcntl(m_socket, F_GETFL, 0)) < 0 || fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::exception;
    
    pthread_t threads[in_thread_count];
    for (int i=0; i < in_thread_count; ++i)
    {
        struct event_base *ev_base = event_init();
        if (ev_base == NULL) 
            throw std::exception;
        struct evhttp *ev_httpd = evhttp_new(ev_base);
        if (ev_httpd == NULL)
            throw std::exception;
        result = evhttp_accept_socket(ev_httpd, m_socket);
        if (result != 0)
            throw std::exception;
        evhttp_set_gencb(ev_httpd, http_server::generic_callback, this);
        result = pthread_create(&threads[i], NULL, http_server::dispatch, ev_base);
        if (result != 0)
            throw std::exception;
    }
    for (int i = 0; i < in_thread_count; ++i) 
    {
        pthread_join(threads[i], NULL);
    }
}

http_server::~http_server( )
{
    std::cerr << m_evhttp << std::endl;
}

void* http_server::dispatch(void* in_arg)
{
    event_base_dispatch((struct event_base*)in_arg);
    return NULL;
}

void http_server::generic_callback(evhttp_request* in_request, void* in_arg)
{
    ((http_server*)in_arg)->process_request(in_request);
}

void http_server::process_request(evhttp_request* in_request)
{
    struct evbuffer *l_buffer = evbuffer_new();
    if (l_buffer == NULL) 
        throw std::exception;
    evbuffer_add_printf(l_buffer, "Requested: %s\n", evhttp_request_uri(in_request));
    evhttp_send_reply(in_request, HTTP_OK, "OK", l_buffer);
}