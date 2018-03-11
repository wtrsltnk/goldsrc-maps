/*
 * Copyright (C) 2009  Wouter Saaltink

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TOKENIZER_H
#define	_TOKENIZER_H

#include <string>

class Tokenizer
{
public:
    Tokenizer(const std::string& data);
    virtual ~Tokenizer();

    bool next();
    const std::string& get() const;
    float getAsFloat() const;
    
private:
    std::string _data;
    int _cursor;
    std::string _token;

};

#endif	/* _TOKENIZER_H */

