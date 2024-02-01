#include "PcapFileReader.hpp"

#include <Packet.h>

#include <iostream>

#include "CustomProtoFilter.hpp"
#include "PacketClassifier.hpp"

PcapFileReader::PcapFileReader(std::string filename) {
    _reader = pcpp::IFileReaderDevice::getReader(filename);

    // verify that a reader interface was indeed created
    if (_reader == NULL) {
        throw std::invalid_argument("Cannot determine reader for file type");
    }

    if (!_reader->open()) {
        throw std::runtime_error("Cannot open reader for this file");
    }

    auto protoFilter = CustomProtoFilter::getFilter();
    _reader->setFilter(*protoFilter);
}

PcapFileReader& PcapFileReader::getInstance(std::string filename) {
    static PcapFileReader instance(filename);
    return instance;
}

void PcapFileReader::read() {
    pcpp::RawPacket rawPacket;

    while (_reader->getNextPacket(rawPacket)) {
        int length = rawPacket.getRawDataLen();

        pcpp::Packet parsed(&rawPacket);
        Flow flow = PacketClassifier::extractFlow(parsed);

        _stats.consumePacket(flow, length);
    }
}
