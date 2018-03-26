#include "mapparser.h"
#include "mapentity.h"
#include "mapbrush.h"
#include "mapbrushside.h"
#include "mapscene.h"
#include "mapkeyvaluepair.h"
#include "tokenizer.h"

#include <fstream>
#include <streambuf>
#include <functional>
#include <glm/glm.hpp>

MapParser::MapParser(const std::string& content)
    : _tok(nullptr), _logger([] (LogLevels level, const std::string& message) { })
{
    this->_tok = new Tokenizer(content);
}

bool MapParser::returnFalseAndLog(const std::string& message)
{
    this->_logger(LogLevels::Error, message);
    return false;
}

bool MapParser::LoadScene(MapScene* scene)
{
    // Start reading all entities
    while (this->_tok->next())
    {
        if (this->_tok->get() == "{")
        {
            auto entity = new MapEntity();
            if (this->LoadEntity(entity))
            {
                scene->_entities.push_back(entity);
            }
            else
            {
                delete entity;
                return returnFalseAndLog("Failed to load entity");
            }
        }
        else
        {
            return returnFalseAndLog("Unexpected end of file");
        }
    }

    scene->_flags |= SceneFlags::Modified;

    return true;
}

bool MapParser::LoadEntity(MapEntity* entity)
{
    while (this->_tok->next() && this->_tok->get() != "}")
    {
        if (this->_tok->get() == "{")
        {
            auto brush = new MapBrush();
            if (this->LoadBrush(brush))
            {
                entity->_brushes.push_back(brush);
            }
            else
            {
                delete brush;
                return returnFalseAndLog("Failed to load brush");
            }
        }
        else
        {
            std::string key(this->_tok->get());
            if (!this->_tok->next()) return false;

            std::string value(this->_tok->get());

            entity->_keyValuePairs.push_back(new MapKeyValuePair(key, value));
        }
    }

    return true;
}

bool MapParser::LoadBrush(MapBrush* brush)
{
    this->_tok->next();

    while (this->_tok->get() != "}")
    {
        glm::vec3 v1, v2, v3;

        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "("
        v1.x = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v1.y = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v1.z = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the ")"

        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "("
        v2.x = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v2.y = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v2.z = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the ")"

        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "("
        v3.x = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v3.y = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        v3.z = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the ")"

        auto side = MapBrushSide::FromVertices(v1, v2, v3);

        side._material._texture = this->_tok->get();
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Texture name

        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "["
        side._material._axis1.x = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// tx1
        side._material._axis1.y = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// ty1
        side._material._axis1.z = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// tz1
        side._material._offset1 = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// toffs1
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "]"

        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "["
        side._material._axis2.x = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// tx2
        side._material._axis2.y = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// ty2
        side._material._axis2.z = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// tz2
        side._material._offset2 = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// toffs2
        if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// Skip the "]"

        side._material._rotation = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// rotation
        side._material._scalex = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// scaleX
        side._material._scaley = this->_tok->getAsFloat(); if (this->_tok->next() == false) return returnFalseAndLog("Unexpected end of file");	// scaleY

        brush->_sides.push_back(side);
    }

    return true;
}

void MapParser::SetLogger(std::function<void (LogLevels, const std::string&)> logger)
{
    this->_logger = logger;
}
