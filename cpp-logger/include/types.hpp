#pragma once

#include <string>
#include <iostream>

struct Address
{
    std::string host;
    uint16_t port;

    bool operator==(const Address &other) const;
};

struct Connection
{
    Address src;
    Address dst;

    bool operator==(const Connection &other) const;
};

std::ostream &operator<<(std::ostream &os, const Address &address);

std::ostream &operator<<(std::ostream &os, const Connection &connection);

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
