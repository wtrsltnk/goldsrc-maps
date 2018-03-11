#include "tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(const std::string& data)
    : _data(data), _cursor(0), _token("")
{ }

Tokenizer::~Tokenizer() { }

bool isSpaceCharacter(const char c)
{
    return (c <= ' ');
}

bool Tokenizer::next()
{
    this->_token = "";

    while (this->_cursor <= this->_data.size() && isSpaceCharacter(this->_data[this->_cursor]))
        this->_cursor++;

    if (this->_data[this->_cursor] == '\"')
    {
        this->_cursor++;

        while (this->_cursor <= this->_data.size() && this->_data[this->_cursor] != '\"')
        {
            this->_token += this->_data[this->_cursor];
            this->_cursor++;
        }

        this->_cursor++;
    }
    else
    {
        while (this->_cursor <= this->_data.size() && !isSpaceCharacter(this->_data[this->_cursor]))
        {
            this->_token += this->_data[this->_cursor];
            this->_cursor++;
        }
    }

    return this->_token.size() > 0;
}

const std::string& Tokenizer::get() const
{
    return this->_token;
}

float Tokenizer::getAsFloat() const
{
    return atof(this->_token.c_str());
}
