#include "PcapLiveReader.hpp"

#include <Packet.h>

#include <iostream>

#include "CustomProtoFilter.hpp"
#include "PacketClassifier.hpp"

PcapLiveReader::PcapLiveReader(std::string interface) {
    _device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interface);

    // verify that a device reader was indeed created
    if (_device == NULL) {
        throw std::invalid_argument("Cannot find provided interface");
    }

    if (!_device->open()) {
        throw std::runtime_error("Cannot open reader for this device");
    }

    auto protoFilter = CustomProtoFilter::getFilter();
    _device->setFilter(*protoFilter);
}

PcapLiveReader& PcapLiveReader::getInstance(std::string interface) {
    static PcapLiveReader instance(interface);
    return instance;
}

void PcapLiveReader::read() {}

void PcapLiveReader::listPcapLiveDevices() {
    std::vector<pcpp::PcapLiveDevice*> devices =
        pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    for (pcpp::PcapLiveDevice* device : devices) {
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