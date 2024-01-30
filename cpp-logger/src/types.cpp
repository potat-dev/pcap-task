#include "types.hpp"
#include "utils.hpp"

#include <sstream>

bool Address::operator==(const Address &other) const
{
    return (host == other.host && port == other.port);
}

std::string Address::toString() const
{
    std::ostringstream out;
    out << host << ":" << port;
    return out.str();
}

std::ostream &operator<<(std::ostream &os, const Address &address)
{
    os << address.toString();
    return os;
}

bool Connection::operator==(const Connection &other) const
{
    return (src == other.src && dst == other.dst);
}

std::string Connection::toString() const
{
    std::ostringstream out;
    out << src << " => " << dst;
    return out.str();
}

std::ostream &operator<<(std::ostream &os, const Connection &connection)
{
    os << connection.toString();
    return os;
}

std::string Stats::toString() const
{
    std::ostringstream out;
    out << "Count: " << count << ", Bytes: " << bytes;
    return out.str();
}

std::ostream &operator<<(std::ostream &os, const Stats &stats)
{
    os << stats.toString();
    return os;
}

namespace std
{
    size_t hash<Address>::operator()(const Address &object) const
    {
        size_t seed = 0;
        hash_combine(seed, object.host);
        hash_combine(seed, object.port);
        return seed;
    }

    size_t hash<Connection>::operator()(const Connection &object) const
    {
        size_t seed = 0;
        hash_combine(seed, object.src);
        hash_combine(seed, object.dst);
        return seed;
    }
}
