#pragma once

#include <iostream>
#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>

#include "types.hpp"

// custom type utils

template <class T>
inline void hash_combine(std::size_t &seed, const T &v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

bool saveStatsAsCSV(ConnectionStats &stats, std::string filename);

// pcap utils

void listPcapLiveDevices();

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType);
