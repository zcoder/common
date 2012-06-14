/* 
 * File:   c_evbuffer.cpp
 * Author: zhenka
 * 
 * Created on 11 Июнь 2012 г., 21:55
 */

#include "c_evbuffer.h"

#include <iostream>

c_evbuffer::c_evbuffer( ):
m_buffer(evbuffer_new())
{
    if (m_buffer == NULL)
        throw std::exception();
}

c_evbuffer::c_evbuffer( struct evbuffer *o_buffer ):
m_buffer(o_buffer)
{
    if (m_buffer == NULL)
        throw std::exception();
}


c_evbuffer::c_evbuffer( c_evbuffer& orig ):
m_buffer(orig)
{
}

c_evbuffer::operator struct evbuffer* ()
{
    return m_buffer;
}

c_evbuffer::~c_evbuffer( )
{
    evbuffer_free(m_buffer);
}

