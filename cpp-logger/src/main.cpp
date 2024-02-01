#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>

#include <CLI/CLI.hpp>
#include <iostream>

#include "PcapFileReader.hpp"

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

    if (!listInterfaces && (input.empty() || output.empty())) {
        std::cerr << "You need to specify input and output" << std::endl;
    }

    AbstractReader* reader;

    try {
        reader = &PcapFileReader::getInstance(input);
    } catch (std::exception e) {
        std::cerr << "Unable to open reader:" << std::endl << e.what() << std::endl;
        return 1;
    }

    reader->read();
    reader->close();

    try {
        reader->saveToCSV(output);
    } catch (std::exception e) {
        std::cerr << "Unable to save CSV:" << std::endl << e.what() << std::endl;
        return 1;
    }

    // reader->saveToCSV(output);

    // std::cout << "\n-------- FINAL LOG ---------\n" << std::endl;

    // for (auto i : stats.asMap()) {
    //     std::cout << i.first.getSrcHost() << " : " << i.first.getSrcPort() << " -> "
    //               << i.first.getDstHost() << " : " << i.first.getDstPort() << " -- "
    //               << "Packets: " << i.second.getPackets() << " Bytes: " << i.second.getBytes()
    //               << std::endl;
    // }

    std::cout << "Saved to the file" << std::endl;

    return 0;
}
