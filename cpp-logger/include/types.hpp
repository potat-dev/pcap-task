#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

// type definition

struct Address
{
    std::string host;
    uint16_t port;

    bool operator==(const Address &other) const;
    std::string toString() const;
};

struct Connection
{
    Address src;
    Address dst;

    bool operator==(const Connection &other) const;
    std::string toString() const;
};

struct Stats
{
    uint32_t count;
    uint64_t bytes;

    std::string toString() const;
};

typedef std::unordered_map<Connection, Stats> ConnectionStats;

// methods definition

std::ostream &operator<<(std::ostream &os, const Address &address);

std::ostream &operator<<(std::ostream &os, const Connection &connection);

std::ostream &operator<<(std::ostream &os, const Stats &stats);

namespace std
{
    template <>
    struct hash<Address>
    {
        auto operator()(const Address &object) const -> size_t;
    };

    template <>
    struct hash<Connection>
    {
        auto operator()(const Connection &object) const -> size_t;
    };
}
