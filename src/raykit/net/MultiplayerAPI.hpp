#pragma once

#include <enet.h>
#include <string>

class MultiplayerAPI 
{
    private:
        bool initialized;
    public:
        MultiplayerAPI();
        ~MultiplayerAPI();
    public:
        ENetHost* create_server(int port, int maxClients);
        ENetHost* create_client(const std::string& host, int port);
};
