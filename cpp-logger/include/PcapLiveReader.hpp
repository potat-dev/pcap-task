#pragma once

#include "AbstractReader.hpp"
#include "PcapLiveDeviceList.h"

class PcapLiveReader : public AbstractReader {
   private:
    pcpp::PcapLiveDevice* _device;

   public:
    PcapLiveReader(std::string interface);
    void read() override;

    static void listPcapLiveDevices();
};
