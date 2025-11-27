#pragma once

#include "game/net/PacketType.hpp"
#include <vector>

struct RawPacket
{
    PacketType type;
    std::vector<uint8_t> payload;
};
