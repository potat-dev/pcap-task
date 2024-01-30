#include <IPv4Layer.h>
#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "FlowStats.hpp"
#include "PacketClassifier.hpp"
#include "utils.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: app <file.pcap>" << std::endl;
        return 0;
    }
    // open a pcap file for reading
    pcpp::PcapFileReaderDevice reader(argv[1]);
    // pcpp::PcapFileReaderDevice reader("/code/projects/pcap-task/test.pcap");

    if (!reader.open()) {
        std::cerr << "Error opening the pcap file" << std::endl;
        return 1;
    }

    pcpp::ProtoFilter ipFilter(pcpp::IPv4);
    pcpp::ProtoFilter tcpFilter(pcpp::TCP);
    pcpp::ProtoFilter udpFilter(pcpp::UDP);

    pcpp::OrFilter transportFilter;
    transportFilter.addFilter(&tcpFilter);
    transportFilter.addFilter(&udpFilter);

    pcpp::AndFilter protoFilter;
    protoFilter.addFilter(&ipFilter);
    protoFilter.addFilter(&transportFilter);

    reader.setFilter(protoFilter);

    FlowStats stats;

    pcpp::RawPacket rawPacket;

    while (reader.getNextPacket(rawPacket)) {
        auto length = rawPacket.getRawDataLen();
        // parse the raw packet into a parsed packet
        pcpp::Packet parsed(&rawPacket);

        // verify the packet is IPv4
        if (parsed.isPacketOfType(pcpp::IPv4) &&
            (parsed.isPacketOfType(pcpp::TCP) || parsed.isPacketOfType(pcpp::UDP))) {
            Flow flow = PacketClassifier::extractFlow(parsed);
            stats.consumePacket(flow, length);
        }
    }

    reader.close();

    std::cout << "\n-------- FINAL LOG ---------\n" << std::endl;

    for (auto i : stats.asMap()) {
        std::cout << i.first.getSrcHost() << " : " << i.first.getSrcPort() << " -> "
                  << i.first.getDstHost() << " : " << i.first.getDstPort() << " -- "
                  << "Packets: " << i.second.getPackets() << " Bytes: " << i.second.getBytes()
                  << std::endl;
    }

    // save to file
    // if (!saveStatsAsCSV(stats, "/code/projects/pcap-task/test.csv")) {
    //     std::cout << "Can't save to the file" << std::endl;
    //     exit(1);
    // }
    // std::cout << "Saved to the file" << std::endl;

    return 0;
}
