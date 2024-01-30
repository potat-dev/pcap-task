#pragma once

#include <cstdint>

class Stats {
   private:
    uint32_t _packets;
    uint64_t _bytes;

   public:
    Stats();

    uint32_t getPackets() const;
    uint64_t getBytes() const;

    void increment(uint64_t bytes);
};
