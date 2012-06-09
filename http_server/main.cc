/* 
 * File:   main.cc
 * Author: zhenka
 *
 * Created on 9 Июнь 2012 г., 6:04
 */

#include <cstdlib>
#include "http_server.h"

using namespace std;

/*
 * 
 */
int main( int argc, char** argv )
{
    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " port threads" << endl;
        exit(1);
    }
    
    int r;
    int nfd;
    uint16_t port = (uint16_t)atoi(argv[1]);
    nfd = socket(AF_INET, SOCK_STREAM, 0);
    if (nfd < 0) 
    {   
        cerr << "cant create socket" << endl;
        exit(1);
    }
    int one = 1;
    r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) 
    {
        cerr << "cant bind" << endl;
        exit(1);
    }
    r = listen(nfd, 10240);
    if (r < 0)
    {
        cerr << "cant listen" << endl;
        exit(1);

    }

    int flags;
    if ((flags = fcntl(nfd, F_GETFL, 0)) < 0 || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        cerr << "bag flags" << endl;
        exit(1);

    }
    
    uint32_t nthreads = (u_int16_t)atoi(argv[2]);
    pthread_t ths[nthreads];
    http_server *http_serv[nthreads];
    for (int i = 0; i < nthreads; i++) 
    {
        http_serv[i] = new http_server( nfd );
        evhttp_set_cb(ev_httpd, "/get", http_get_cb, NULL);
        r = pthread_create(&ths[i], NULL, pthread_event_base_dispatch, ev_base);
        if (r != 0)
        {
            cerr << "cant pthread_create()" << endl;
            exit(1);
        }
    }
    
    for (int i = 0; i < nthreads; i++) 
    {
        pthread_join(ths[i], NULL);
    }
    
    return 0;
}

