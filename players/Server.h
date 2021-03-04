#pragma once

#include "NetworkPlayer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

/**
 * A server player.
 * Listens on a socket for the other player.
 */
class Server : public NetworkPlayer {
public:

    /**
     * Constructor.
     *
     * @param address   The mask address to listen on.
     * @param port      The port to listen on.
     */
    Server(const string &address, int port);

    /**
     * Destructor.
     */
    ~Server();


private:

    /// The socket on which the binding and listening is done.
    int m_server_socket;
};
