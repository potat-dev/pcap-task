#pragma once

#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>

#include <iostream>

// boost hashing function

template <class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// pcap utils

void listPcapLiveDevices();
