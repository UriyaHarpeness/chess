#pragma once

#include "NetworkPlayer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

class Client : public NetworkPlayer {
public:
    Client(const string &address, int port);

    ~Client() = default;
};
