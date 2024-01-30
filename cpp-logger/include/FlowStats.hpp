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

    FlowStatsT asMap() const;
    void consumePacket(Flow &flow, uint64_t bytes);
    bool saveAsCSV(std::string filename);
};
