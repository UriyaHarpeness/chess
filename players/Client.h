#pragma once

#include "NetworkPlayer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

/**
 * A client player.
 * Connects to a socket to the other player.
 */
class Client : public NetworkPlayer {
public:

    /**
     * Constructor.
     *
     * @param address   The address to connect to.
     * @param port      The port to connect to.
     */
    Client(const string &address, int port);

    /**
     * Destructor.
     */
    ~Client() = default;
};
