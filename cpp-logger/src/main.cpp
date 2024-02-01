#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>

#include <CLI/CLI.hpp>
#include <iostream>

#include "CustomProtoFilter.hpp"
#include "FlowStats.hpp"
#include "PacketClassifier.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{"Packet classifier"};
    app.require_subcommand(1);

    // settings
    std::string input, output;
    bool listInterfaces = false;

    // initialize subcommands
    CLI::App* appFileMode = app.add_subcommand("file", "File mode - read from .pcap file");
    CLI::App* appLiveMode = app.add_subcommand("live", "Live mode - capture from interface");

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
    CLI::Option* listInterfacesOption =
        appLiveMode->add_flag("-l,--list", listInterfaces, "List available interfaces and exit");
    CLI::Option* liveInputOption =
        appLiveMode->add_option("-i,--input", input, "Input interface")->option_text("FILE");
    CLI::Option* liveOutputOption =
        appLiveMode->add_option("-o,--output", output, "Output .csv file")->option_text("FILE");

    // requirements
    liveInputOption->needs(liveOutputOption);
    liveOutputOption->needs(liveInputOption);

    appLiveMode->final_callback([]() { std::cout << "LIVE MODE !!!" << std::endl; });

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

    // compose filters: IPv4 and (TCP or UDP)

    auto protoFilter = CustomProtoFilter::getFilter();
    reader.setFilter(*protoFilter);

    FlowStats stats;

    pcpp::RawPacket rawPacket;

    while (reader.getNextPacket(rawPacket)) {
        int length = rawPacket.getRawDataLen();
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
    if (!stats.saveToCSV(output)) {
        std::cout << "Can't save to the file" << std::endl;
        exit(1);
    }

    std::cout << "Saved to the file" << std::endl;

    return 0;
}
