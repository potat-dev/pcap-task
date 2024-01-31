#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>

#include <CLI/CLI.hpp>
#include <iostream>

#include "FlowStats.hpp"
#include "PacketClassifier.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{"Packet classifier"};
    app.require_subcommand(1);

    // settings
    std::string input, output;
    bool listInterfaces = false;

    // initialize subcommands
    CLI::App* appFileMode = app.add_subcommand("file", "File mode");
    CLI::App* appInterfaceMode = app.add_subcommand("interface", "Interface mode");

    // requirements
    appFileMode->require_option(1, 2);
    appInterfaceMode->require_option(1, 2);

    // file mode
    CLI::Option* fileInputOption =
        appFileMode->add_option("-i,--input", input, "Input .pcap file")->option_text("FILE");
    CLI::Option* fileOutputOption =
        appFileMode->add_option("-o,--output", output, "Output .csv file")->option_text("FILE");

    // requirements
    fileInputOption->needs(fileOutputOption);
    fileOutputOption->needs(fileInputOption);

    appFileMode->final_callback([]() { std::cout << "FILE MODE !!!" << std::endl; });

    // interface mode
    CLI::Option* interfaceInputOption =
        appInterfaceMode->add_option("-i,--input", input, "Input interface")->option_text("FILE");
    CLI::Option* interfaceOutputOption =
        appInterfaceMode->add_option("-o,--output", output, "Output .csv file")
            ->option_text("FILE");
    CLI::Option* listiInterfacesOption =
        appInterfaceMode->add_flag("-l,--list", listInterfaces, "List interfaces");

    // requirements
    interfaceInputOption->needs(interfaceOutputOption);
    interfaceOutputOption->needs(interfaceInputOption);
    listiInterfacesOption->excludes(interfaceInputOption);
    listiInterfacesOption->excludes(interfaceOutputOption);

    appInterfaceMode->final_callback([]() { std::cout << "INTERFACE MODE !!!" << std::endl; });

    // actual parsing
    CLI11_PARSE(app, argc, argv);

    std::cout << "In: " << input << ", Out: " << output << std::endl;
    std::cout << "--- parsing complete !!! ---" << std::endl;

    // open a pcap file for reading
    pcpp::PcapFileReaderDevice reader(input);

    if (!reader.open()) {
        std::cerr << "Error opening the pcap file" << std::endl;
        return 1;
    }

    // compose filters:
    // IPv4 and (TCP or UDP)

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
    if (!stats.saveAsCSV(output)) {
        std::cout << "Can't save to the file" << std::endl;
        exit(1);
    }

    std::cout << "Saved to the file" << std::endl;

    return 0;
}
