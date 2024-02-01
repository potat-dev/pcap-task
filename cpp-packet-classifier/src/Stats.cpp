#include "Stats.hpp"

Stats::Stats() : _packets(0), _bytes(0) {}

uint32_t Stats::getPackets() const { return _packets; }
uint64_t Stats::getBytes() const { return _bytes; }

void Stats::increment(uint64_t b) {
    _packets += 1;
    _bytes += b;
}
