/* 
 * File:   main.c++
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
    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " host port threads" << endl;
        exit(1);
    }
    
    in_addr_t addr = inet_addr(argv[1]);
    uint16_t port = (uint16_t)atoi(argv[2]);
    uint32_t threads = (uint32_t)atoi(argv[3]);
    http_server serv(addr, port, threads);
    return 0;
}

