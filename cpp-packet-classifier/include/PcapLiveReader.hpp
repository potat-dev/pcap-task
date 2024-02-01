#pragma once

#include "AbstractReader.hpp"
#include "PcapLiveDeviceList.h"

class PcapLiveReader : public AbstractReader {
   private:
    pcpp::PcapLiveDevice* _device;
    static bool _capturing;

    PcapLiveReader(std::string interface);
    PcapLiveReader(const PcapLiveReader&) = delete;
    PcapLiveReader& operator=(const PcapLiveReader&) = delete;

    static void packetHandler(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie);
    static void signalHandler(int sig);

   public:
    static PcapLiveReader& getInstance(std::string interface);

    void init() override;
    void read() override;
    inline void close() override { _device->close(); }

    static void listPcapLiveDevices();
};
