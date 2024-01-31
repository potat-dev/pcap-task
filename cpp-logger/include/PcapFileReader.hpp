#pragma once

#include "AbstractReader.hpp"
#include "PcapFileDevice.h"

class PcapFileReader : public AbstractReader {
   private:
    pcpp::IFileReaderDevice* _reader;

   public:
    PcapFileReader(std::string filename);
    void read() override;
};
