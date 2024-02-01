#pragma once

#include "AbstractReader.hpp"
#include "PcapLiveDeviceList.h"

class PcapLiveReader : public AbstractReader {
   private:
    pcpp::PcapLiveDevice* _device;

    PcapLiveReader(std::string interface);
    PcapLiveReader(const PcapLiveReader&) = delete;
    PcapLiveReader& operator=(const PcapLiveReader&) = delete;

   public:
    static PcapLiveReader& getInstance(std::string interface);

    void read() override;
    inline void close() { _device->close(); }

    static void listPcapLiveDevices();
};
