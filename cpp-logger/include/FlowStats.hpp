#pragma once

#include <unordered_map>

#include "Flow.hpp"
#include "Stats.hpp"

typedef std::unordered_map<Flow, Stats> FlowStatsT;

class FlowStats {
   private:
    FlowStatsT _stats;

   public:
    FlowStats();

    void consumePacket(Flow &flow, uint64_t bytes);
    FlowStatsT asMap() const;
};
