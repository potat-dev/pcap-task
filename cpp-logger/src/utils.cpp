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

// bool saveStatsAsCSV(ConnectionStats &stats, std::string filename) {
//     std::ofstream file(filename);
//     if (!file.is_open()) {
//         return false;  // Unable to open the file
//     }

//     // CSV header
//     file << "Source Host,Destination Host,Source Port,Destination Port,Count,Bytes\n";

//     for (const auto &entry : stats) {
//         const Connection &conn = entry.first;
//         const Stats &stats = entry.second;

//         file << conn.src.host << "," << conn.dst.host << "," << conn.src.port << ","
//              << conn.dst.port << "," << stats.count << "," << stats._bytes << "\n";
//     }

//     file.close();
//     return true;
// }

// pcap utils

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType) {
    switch (protocolType) {
        case pcpp::Ethernet:
            return "Ethernet";
        case pcpp::IPv4:
            return "IPv4";
        case pcpp::TCP:
        case pcpp::UDP:
            return "TCP";
        case pcpp::HTTPRequest:
        case pcpp::HTTPResponse:
            return "HTTP";
        default:
            return "Unknown";
    }
}
