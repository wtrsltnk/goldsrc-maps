#ifndef MAPKEYVALUEPAIR_H
#define MAPKEYVALUEPAIR_H

#include <string>

class MapKeyValuePair
{
public:
    MapKeyValuePair();
    MapKeyValuePair(const MapKeyValuePair& other);
    MapKeyValuePair(const std::string& key, const std::string& value);

    MapKeyValuePair& operator = (const MapKeyValuePair& other);

    std::string _key;
    std::string _value;
};

#endif // MAPKEYVALUEPAIR_H
