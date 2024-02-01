#pragma once

#include "FlowStats.hpp"

class AbstractReader {
   protected:
    FlowStats _stats;

   public:
    virtual void read() = 0;
    virtual void close() = 0;
    inline bool saveToCSV(std::string filename) { return _stats.saveToCSV(filename); }
};
