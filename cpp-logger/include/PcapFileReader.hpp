#pragma once

#include "AbstractReader.hpp"
#include "PcapFileDevice.h"

class PcapFileReader : public AbstractReader {
   private:
    pcpp::IFileReaderDevice* _reader;

    PcapFileReader(std::string filename);
    PcapFileReader(const PcapFileReader&) = delete;
    PcapFileReader& operator=(const PcapFileReader&) = delete;

   public:
    static PcapFileReader& getInstance(std::string filename);

    void init() override;
    void read() override;
    inline void close() override { _reader->close(); }
};
