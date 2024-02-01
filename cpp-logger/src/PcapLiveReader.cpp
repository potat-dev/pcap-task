#include "PcapLiveReader.hpp"

#include <iostream>

PcapLiveReader::PcapLiveReader(std::string interface) {
    _device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interface);

    // verify that a device reader was indeed created
    if (_device == NULL) {
        throw std::invalid_argument("Cannot find provided interface");
    }
}

void PcapLiveReader::read() {}
