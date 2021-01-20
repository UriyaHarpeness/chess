#pragma once

#include "NetworkPlayer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

class Server : public NetworkPlayer {
public:
    Server(const string &address, int port);

    ~Server();

private:
    int m_server_socket;
};
