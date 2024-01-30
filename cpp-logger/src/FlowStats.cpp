#include "FlowStats.hpp"

FlowStats::FlowStats() {}

void FlowStats::consumePacket(Flow &flow, uint64_t bytes) { _stats[flow].increment(bytes); }

FlowStatsT FlowStats::asMap() const { return _stats; }