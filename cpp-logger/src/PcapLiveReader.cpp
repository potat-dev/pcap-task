#include "PcapLiveReader.hpp"

PcapLiveReader::PcapFileReader(std::string interface) {
    _device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interface);
    if (dev == NULL) {
        std::cerr << "Cannot find interface with IPv4 address of '" << interfaceIPAddr << "'"
                  << std::endl;
        return 1;
    }
}
