#include "mapkeyvaluepair.h"

MapKeyValuePair::MapKeyValuePair() { }

MapKeyValuePair::MapKeyValuePair(const MapKeyValuePair& other) : _key(other._key), _value(other._value) { }

MapKeyValuePair::MapKeyValuePair(const std::string& key, const std::string& value) : _key(key), _value(value) { }

MapKeyValuePair& MapKeyValuePair::operator = (const MapKeyValuePair& other)
{
    this->_key = other._key;
    this->_value = other._value;

    return *this;
}
