#include "PcapFileReader.hpp"

#include <Packet.h>

#include <iostream>

#include "CustomProtoFilter.hpp"
#include "PacketClassifier.hpp"

PcapFileReader::PcapFileReader(std::string filename) {
    _reader = pcpp::IFileReaderDevice::getReader(filename);

    // verify that a reader interface was indeed created
    if (_reader == nullptr) {
        throw std::invalid_argument("Cannot determine reader for file type");
    }
}

PcapFileReader& PcapFileReader::getInstance(std::string filename) {
    static PcapFileReader instance(filename);
    return instance;
}

void PcapFileReader::init() {
    if (!_reader->open()) {
        throw std::runtime_error("Cannot open reader for this file");
    }

    auto protoFilter = CustomProtoFilter::getFilter();
    _reader->setFilter(*protoFilter);
}

void PcapFileReader::read() {
    pcpp::RawPacket rawPacket;

    while (_reader->getNextPacket(rawPacket)) {
        int length = rawPacket.getRawDataLen();

        pcpp::Packet parsedPacket(&rawPacket);
        Flow flow = PacketClassifier::extractFlow(parsedPacket);

        _stats.consumePacket(flow, length);
    }
}
