/*
 * tcp_sock.h
 *
 *  Created on: Aug 26, 2018
 *      Author: charles
 */

#ifndef TCP_SOCK_H_
#define TCP_SOCK_H_

#include <iostream>
#include <stdio.h>
#include <string.h>    //strlen
#include <string>  //string
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent

#include "parserInfo.h"


using namespace std;

/**
    TCP Client class
*/
class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    tcp_client();
    int conn(string, int);
    bool send_data(jobInfo_t *data);
    bool receive(jobInfo_t *data);
};






#endif /* TCP_SOCK_H_ */
