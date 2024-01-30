#include "Flow.hpp"

#include "utils.hpp"

Flow::Flow(std::string srcHost, std::string dstHost, uint16_t srcPort, uint16_t dstPort)
    : srcHost(srcHost), dstHost(dstHost), srcPort(srcPort), dstPort(dstPort) {}

std::string Flow::getSrcHost() const { return srcHost; }

std::string Flow::getDstHost() const { return dstHost; }

uint16_t Flow::getSrcPort() const { return srcPort; }

uint16_t Flow::getDstPort() const { return dstPort; }

bool Flow::operator==(const Flow& other) const {
    return (srcHost == other.srcHost) && (dstHost == other.dstHost) && (srcPort == other.srcPort) &&
           (dstPort == other.dstPort);
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
