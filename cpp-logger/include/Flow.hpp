#pragma once

#include <string>

class Flow {
   private:
    std::string srcHost;
    std::string dstHost;
    uint16_t srcPort;
    uint16_t dstPort;

   public:
    Flow(std::string srcHost, std::string dstHost, uint16_t srcPort, uint16_t dstPort);

    std::string getSrcHost() const;
    std::string getDstHost() const;
    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;

    bool operator==(const Flow& other) const;

    friend std::hash<Flow>;
};

namespace std {
template <>
struct hash<Flow> {
    size_t operator()(const Flow& object) const;
};
}  // namespace std
