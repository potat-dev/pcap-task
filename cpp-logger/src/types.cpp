#include "types.hpp"
#include "utils.hpp"

bool Address::operator==(const Address &other) const
{
    return (host == other.host && port == other.port);
}

bool Connection::operator==(const Connection &other) const
{
    return (src == other.src && dst == other.dst);
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
