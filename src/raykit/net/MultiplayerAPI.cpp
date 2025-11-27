#include "game/net/PacketType.hpp"
#include <cstddef>
#include <cstdint>
#include <raykit/net/MultiplayerAPI.hpp>

#include <enet.h>
#include <vector>

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

bool MultiplayerAPI::send_packet(ENetPeer* peer, const RawPacket& packet)
{
    if(!peer) {
        return false;
    }

    if(!this->initialized) {
        return false;
    }

    size_t total_size = sizeof(PacketType) + packet.payload.size();
    std::vector<uint8_t> buffer(total_size);
    
    std::memcpy(buffer.data(), &packet.type, sizeof(PacketType));
    std::memcpy(buffer.data() + sizeof(PacketType),
                packet.payload.data(),
                packet.payload.size());

    ENetPacket* enetPkt =
        enet_packet_create(buffer.data(), buffer.size(),
                           ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(peer, 0, enetPkt);
    enet_host_flush(peer->host); 

    return true;
}

bool MultiplayerAPI::send_packet_unreliable(ENetPeer* peer, const RawPacket& packet)
{
    if(!peer) {
        return false;
    }
    if(!this->initialized) {
        return false;
    }

    size_t total_size = sizeof(PacketType) + packet.payload.size();
    std::vector<uint8_t> buffer(total_size);
    
    std::memcpy(buffer.data(), &packet.type, sizeof(PacketType));
    std::memcpy(buffer.data() + sizeof(PacketType),
                packet.payload.data(),
                packet.payload.size());
    ENetPacket* enetPkt =
        enet_packet_create(buffer.data(), buffer.size(),
                           ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(peer, 0, enetPkt);
    enet_host_flush(peer->host); 
    return true;
}

bool MultiplayerAPI::send_broadcast(ENetHost* host, const RawPacket& packet)
{
    if(!host) {
        return false;
    }
    if(!this->initialized) {
        return false;
    }
    size_t total_size = sizeof(PacketType) + packet.payload.size();
    std::vector<uint8_t> buffer(total_size);
    
    std::memcpy(buffer.data(), &packet.type, sizeof(PacketType));
    std::memcpy(buffer.data() + sizeof(PacketType),
                packet.payload.data(),
                packet.payload.size());
    ENetPacket* enetPkt =
        enet_packet_create(buffer.data(), buffer.size(),
                           ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(host, 0, enetPkt);
    enet_host_flush(host); 
    return true;
}
void MultiplayerAPI::poll_events(ENetHost* host, std::function<void(ENetEvent&, const RawPacket&)>& on_packet)
{
    if(!host) {
        return;
    }
    if(!this->initialized) {
        return;
    }

    ENetEvent event;
    while(enet_host_service(host, &event, 0) > 0) {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            if(this->peer_connected) {
                this->peer_connected(event.peer->incomingPeerID);
            }
        }
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            RawPacket packet;
            size_t payload_size = event.packet->dataLength - sizeof(PacketType);
            packet.payload.resize(payload_size);
            std::memcpy(&packet.type, event.packet->data, sizeof(PacketType));
            std::memcpy(packet.payload.data(),
                        event.packet->data + sizeof(PacketType),
                        payload_size);
            on_packet(event, packet);
            enet_packet_destroy(event.packet);
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            if(this->peer_disconnected) {
                this->peer_disconnected(event.peer->incomingPeerID);
            }
        }
    }
}


