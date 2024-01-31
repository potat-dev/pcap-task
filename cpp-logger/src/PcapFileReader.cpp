#include "PcapFileReader.hpp"

#include <iostream>

PcapFileReader::PcapFileReader(std::string filename) {
    _reader = pcpp::IFileReaderDevice::getReader(filename);

    // verify that a reader interface was indeed created
    if (_reader == NULL) {
        throw std::invalid_argument("Cannot determine reader for file type");
    }
}

void PcapFileReader::read() {}
