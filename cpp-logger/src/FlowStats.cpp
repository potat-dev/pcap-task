#include "FlowStats.hpp"

#include <fstream>

FlowStats::FlowStats() {}

FlowStatsT FlowStats::asMap() const { return _stats; }

void FlowStats::consumePacket(Flow &flow, uint64_t bytes) { _stats[flow].increment(bytes); }

bool FlowStats::saveToCSV(std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;  // unable to open the file
    }

    // CSV header
    file << "Source Host,Destination Host,Source Port,Destination Port,Count,Bytes\n";

    for (const auto &entry : _stats) {
        const Flow &flow = entry.first;
        const Stats &stats = entry.second;

        file << flow.getSrcHost() << "," << flow.getDstHost() << "," << flow.getSrcPort() << ","
             << flow.getDstPort() << "," << stats.getPackets() << "," << stats.getBytes() << "\n";
    }

    file.close();
    return true;
}
