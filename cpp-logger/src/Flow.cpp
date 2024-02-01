#include "Flow.hpp"

#include "utils.hpp"

Flow::Flow(std::string srcHost, std::string dstHost, uint16_t srcPort, uint16_t dstPort)
    : _srcHost(srcHost), _dstHost(dstHost), _srcPort(srcPort), _dstPort(dstPort) {}

std::string Flow::getSrcHost() const { return _srcHost; }

std::string Flow::getDstHost() const { return _dstHost; }

uint16_t Flow::getSrcPort() const { return _srcPort; }

uint16_t Flow::getDstPort() const { return _dstPort; }

bool Flow::operator==(const Flow& other) const {
    return (_srcHost == other._srcHost) && (_dstHost == other._dstHost) &&
           (_srcPort == other._srcPort) && (_dstPort == other._dstPort);
}

namespace std {
size_t hash<Flow>::operator()(const Flow& flow) const {
    size_t seed = 0;
    hash_combine(seed, flow.getSrcHost());
    hash_combine(seed, flow.getDstHost());
    hash_combine(seed, flow.getSrcPort());
    hash_combine(seed, flow.getDstPort());
    return seed;
}
}  // namespace std
