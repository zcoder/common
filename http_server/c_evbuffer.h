/* 
 * File:   c_evbuffer.h
 * Author: zhenka
 *
 * Created on 11 Июнь 2012 г., 21:55
 */

#ifndef C_EVBUFFER_H
#define	C_EVBUFFER_H

#include <event2/buffer.h>
#include <exception>

class c_evbuffer
{
public:
    c_evbuffer( );
    c_evbuffer( struct evbuffer *l_buffer );
    c_evbuffer( c_evbuffer& orig );
   ~c_evbuffer( );
   
   operator struct evbuffer *();
protected:
    struct evbuffer *m_buffer;
} ;

#endif	/* C_EVBUFFER_H */

