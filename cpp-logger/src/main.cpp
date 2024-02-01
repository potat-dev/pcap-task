#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>

#include <CLI/CLI.hpp>
#include <iostream>

#include "PcapFileReader.hpp"
#include "PcapLiveReader.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{"Packet classifier"};
    app.require_subcommand(1);

    // settings
    std::string input, output;

    // initialize subcommands
    CLI::App* appFileMode = app.add_subcommand("file", "File mode - read from .pcap file");
    CLI::App* appLiveMode = app.add_subcommand("live", "Live mode - capture from interface");
    CLI::App* appListInerfacesMode =
        appLiveMode->add_subcommand("list", "List available interfaces and exit");

    // file mode
    CLI::Option* fileInputOption =
        appFileMode->add_option("-i,--input", input, "Input .pcap file")->option_text("FILE");
    CLI::Option* fileOutputOption =
        appFileMode->add_option("-o,--output", output, "Output .csv file")->option_text("FILE");

    // requirements
    fileInputOption->needs(fileOutputOption);
    fileOutputOption->needs(fileInputOption);

    // interface mode
    CLI::Option* liveInputOption =
        appLiveMode->add_option("-i,--input", input, "Input interface")->option_text("FILE");
    CLI::Option* liveOutputOption =
        appLiveMode->add_option("-o,--output", output, "Output .csv file")->option_text("FILE");

    // requirements
    liveInputOption->needs(liveOutputOption);
    liveOutputOption->needs(liveInputOption);

    // actual parsing
    CLI11_PARSE(app, argc, argv);

    // list interfaces mode
    if (appListInerfacesMode->parsed()) {
        std::cout << "Available interfaces:\n" << std::endl;
        PcapLiveReader::listPcapLiveDevices();
        exit(0);
    }

    // ensure i/o arguments are not empty
    if (input.empty() || output.empty()) {
        std::cerr << "You need to specify input and output" << std::endl;
        exit(1);
    }

    // packet reader mode
    AbstractReader* reader;

    try {
        // CLI11 ensures that one of these modes has been selected
        if (app.got_subcommand(appFileMode)) reader = &PcapFileReader::getInstance(input);
        if (app.got_subcommand(appLiveMode)) reader = &PcapLiveReader::getInstance(input);

        // open and initialize reader
        reader->init();
    } catch (std::exception& e) {
        std::cerr << "Unable to open reader:" << std::endl << e.what() << std::endl;
        exit(1);
    }

    try {
        reader->read();
    } catch (std::exception& e) {
        std::cerr << "Unable to read packets:" << std::endl << e.what() << std::endl;
        exit(1);
    }

    reader->close();

    try {
        reader->saveToCSV(output);
    } catch (std::exception& e) {
        std::cerr << "Unable to save CSV:" << std::endl << e.what() << std::endl;
        exit(1);
    }

    std::cout << "Saved to the file" << std::endl;

    return 0;
}
