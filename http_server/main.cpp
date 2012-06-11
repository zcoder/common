/* 
 * File:   main.c++
 * Author: zhenka
 *
 * Created on 9 Июнь 2012 г., 6:04
 */

#include <cstdlib>
#include "http_server.h"


using namespace std;


uint64_t cb_count;
struct timeval start;
/*
 * 
 */
int main( int argc, char** argv )
{
    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " host port threads" << endl;
        exit(1);
    }
    
    gettimeofday(&start, NULL);
    in_addr_t addr = inet_addr(argv[1]);
    uint16_t port = htons((uint16_t)atoi(argv[2]));
    cerr << port << endl;
    uint32_t threads = (uint32_t)atoi(argv[3]);
    http_server serv(addr, port, threads);
    return 0;
}

