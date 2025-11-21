#include <raykit/net/MultiplayerAPI.hpp>

#include <enet.h>

MultiplayerAPI::MultiplayerAPI()
{
    if(enet_initialize() != 0) {
        this->initialized = false;
    } else {
        this->initialized = true;
    }
}

MultiplayerAPI::~MultiplayerAPI()
{
    if(this->initialized) {
        enet_deinitialize();
    }
}

ENetHost* MultiplayerAPI::create_server(int port, int maxClients)
{
    if(!this->initialized) {
        return nullptr;
    }
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    ENetHost* server = enet_host_create(&address, maxClients, 2, 0, 0);
    return server;
}

ENetHost* MultiplayerAPI::create_client(const std::string& host, int port)
{
    if(!this->initialized) {
        return nullptr;
    }
    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if(client == nullptr) {
        return nullptr;
    }
    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;
    ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
    if(peer == nullptr) {
        enet_host_destroy(client);
        return nullptr;
    }
    return client;
}
