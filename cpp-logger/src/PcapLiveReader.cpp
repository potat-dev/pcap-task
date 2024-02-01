#include "PcapLiveReader.hpp"

#include <Packet.h>
#include <SystemUtils.h>
#include <signal.h>

#include <iostream>

#include "CustomProtoFilter.hpp"
#include "PacketClassifier.hpp"

bool PcapLiveReader::_capturing = false;

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

void PcapLiveReader::packetHandler(pcpp::RawPacket* rawPacket, pcpp::PcapLiveDevice* dev,
                                   void* cookie) {
    // extract the stats object form the cookie
    FlowStats* stats = (FlowStats*)cookie;

    int length = rawPacket->getRawDataLen();

    pcpp::Packet parsedPacket(rawPacket);
    Flow flow = PacketClassifier::extractFlow(parsedPacket);

    stats->consumePacket(flow, length);
}

void PcapLiveReader::signalHandler(int sig) { _capturing = false; }

void PcapLiveReader::read() {
    std::cout << std::endl << "Starting async capture..." << std::endl;

    // start capture in async mode. Give a callback function to call to whenever a packet is
    // captured and the stats object as the cookie
    _device->startCapture(packetHandler, &_stats);
    _capturing = true;

    // register Ctrl+C signal handler
    struct sigaction sa;
    sa.sa_handler = PcapLiveReader::signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // wait until Ctrl+C
    while (_capturing) pcpp::multiPlatformSleep(1);

    // reset Ctrl+C signal handler to default
    signal(SIGINT, SIG_DFL);

    // stop capturing packets
    _device->stopCapture();
}

void PcapLiveReader::listPcapLiveDevices() {
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

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