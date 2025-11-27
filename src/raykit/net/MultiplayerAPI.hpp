#pragma once

#include "raykit/net/RawPacket.hpp"
#include <cstdint>
#include <enet.h>
#include <functional>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

class MultiplayerAPI 
{
    private:
        bool initialized;
        std::function<void(int)> peer_connected;
        std::function<void(int)> peer_disconnected;
    public:
        MultiplayerAPI();
        ~MultiplayerAPI();
    public:
        ENetHost* create_server(int port, int maxClients);
        ENetHost* create_client(const std::string& host, int port);
    public:
        bool send_packet(ENetPeer* peer, const RawPacket& packet);
        bool send_packet_unreliable(ENetPeer* peer, const RawPacket& packet);
        bool send_broadcast(ENetHost* host, const RawPacket& packet);
        void poll_events(ENetHost* host, std::function<void(ENetEvent&, const RawPacket&)>& on_packet);
};

template<typename T>
std::vector<uint8_t> serializeStruct(const T& data)
{
    std::vector<uint8_t> buffer(sizeof(T));
    std::memcpy(buffer.data(), &data, sizeof(T));
    return buffer;
}

template<typename T>
T deserializeStruct(const std::vector<uint8_t>& buffer)
{
    if(buffer.size() < sizeof(T)) {
        throw std::runtime_error("Buffer too small to deserialize struct");
    }

    T data;
    std::memcpy(&data, buffer.data(), sizeof(T));
    return data;
}
