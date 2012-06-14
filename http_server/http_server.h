/* 
 * File:   http_server.h
 * Author: zhenka
 *
 * Created on 9 Июнь 2012 г., 6:07
 */

#ifndef HTTP_SERVER_H
#define	HTTP_SERVER_H

#include <cstdlib>
#include <event.h>
#include <evhttp.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <signal.h>

class http_server
{
public:
    http_server( const in_addr_t in_addr, const in_port_t in_port = 20480, const uint32_t in_thread_count = 1 );
    http_server( const int in_socket, const uint32_t in_thread_count = 1 );
    virtual ~http_server();
    virtual void process_request( struct evhttp_request* in_request_ptr );
    virtual std::string find_mime_type(std::string in_ext);
    virtual void fill_mime_type();
private:
    typedef std::map<std::string,std::string> mime_map_t;
    int m_socket;
    mime_map_t m_mime_map;
};

static void* dispatch(void *in_arg);
static void generic_callback( struct evhttp_request *in_request, void *in_arg );

extern uint64_t cb_count;
extern struct timeval start;

#endif	/* HTTP_SERVER_H */

