/* 
 * File:   http_server.c++
 * Author: zhenka
 * 
 * Created on 9 Июнь 2012 г., 6:07
 */

#include "http_server.h"

http_server::http_server( int socket ):
m_evbase(event_init()), m_evhttp(evhttp_new(m_evbase))
{
    if (!m_evbase || !m_evhttp) 
        throw std::exception;
    if (evhttp_accept_socket(m_evhttp, socket))
    {
        throw std::exception;
    }
}

http_server::http_server( const http_server& orig ):
m_evbase(orig.m_evbase), m_evhttp(evhttp_new(m_evbase))
{
    std::cerr << m_evhttp << std::endl;
}

http_server::~http_server( )
{
    std::cerr << m_evhttp << std::endl;
}

