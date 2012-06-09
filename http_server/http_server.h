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

class http_server
{
public:
    http_server( int socket );
    http_server( const http_server& orig );
    virtual uint32_t configure();
    virtual void run();
    virtual void callback_uri( struct evhttp_request *request, void *ctx );
    virtual ~http_server();
protected:
    struct event_base   *m_evbase;
    struct evhttp       *m_evhttp;
private:
    
};

#endif	/* HTTP_SERVER_H */

