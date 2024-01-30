#include "utils.hpp"

#include <fstream>

// pcap utils

void listPcapLiveDevices() {
    std::vector<pcpp::PcapLiveDevice *> devices =
        pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    for (pcpp::PcapLiveDevice *device : devices) {
        std::string desc = device->getDesc();
        if (desc == "") desc = "No description";

        std::cout << "Interface info:" << std::endl
                  << "├╴ Interface name:        " << device->getName() << std::endl
                  << "├╴ Interface description: " << desc << std::endl
                  << "├╴ MAC address:           " << device->getMacAddress() << std::endl
                  << "├╴ IPv4 address:          " << device->getIPv4Address() << std::endl
                  << "└╴ Default gateway:       " << device->getDefaultGateway() << "\n"
                  << std::endl;
    }
}
