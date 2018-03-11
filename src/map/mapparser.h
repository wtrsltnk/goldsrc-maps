#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <string>
#include <functional>

enum class LogLevels
{
    Fatal,
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

class MapParser
{
    bool returnFalseAndLog(const std::string& message);

    class Tokenizer* _tok;
    std::function<void (LogLevels, const std::string&)> _logger;
public:
    MapParser(const std::string& content);

    bool LoadScene(class MapScene* scene);
    bool LoadEntity(class MapEntity* entity);
    bool LoadBrush(class MapBrush* brush);

    void SetLogger(std::function<void (LogLevels, const std::string&)> logger);

};

#endif // MAPPARSER_H
