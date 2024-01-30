#pragma once

#include <Packet.h>

#include "Flow.hpp"

class PacketClassifier {
   public:
    static Flow extractFlow(pcpp::Packet packet);
};
