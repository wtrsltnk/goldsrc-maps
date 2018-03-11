// Make sure we include stdio first to make fopen to work on android
#include <stdio.h>
#include <memory.h>
#include <cstdlib>

#include "shader.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

char* readWholeFile(const std::string& filename)
{
    auto file = fopen(filename.c_str(), "r");
    if (file != nullptr)
    {
        fseek(file, 0, SEEK_END);
        auto size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* buffer = (char*)malloc(size + 1);
        memset(buffer, 0, size + 1);
        fread(buffer, sizeof(char), size + 1, file);
        fclose(file);

        return buffer;
    }

    return nullptr;
}

static std::map<std::string, GLuint> _loadedNamedShaders;

std::string FullShaderPath(const std::string& shaderFileName)
{
    std::stringstream shader;

    shader << SHADER_PATH << shaderFileName;

    return shader.str();
}

GLuint LoadShaderProgram(const char* vertShaderSrc, const char* fragShaderSrc)
{
    if (vertShaderSrc == nullptr)
    {
        return 0;
    }

    if (fragShaderSrc == nullptr)
    {
        return 0;
    }

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    GLint logLength;

    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::cerr << "compiling vertex shader" << std::endl;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> vertShaderError(static_cast<size_t>((logLength > 1) ? logLength : 1));
        glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
        std::cerr << &vertShaderError[0] << std::endl;
    }

    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::cerr << "compiling fragment shader" << std::endl;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> fragShaderError(static_cast<size_t>((logLength > 1) ? logLength : 1));
        glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
        std::cerr << &fragShaderError[0] << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::cerr << "linking program" << std::endl;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> programError(static_cast<size_t>((logLength > 1) ? logLength : 1));
        glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
        std::cerr << &programError[0] << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

GLuint LoadShaderProgram(const std::string& shaderName)
{
    if (_loadedNamedShaders.find(shaderName) != _loadedNamedShaders.end())
    {
        return _loadedNamedShaders[shaderName];
    }

    std::stringstream vert, frag;
    vert << SHADER_PATH << shaderName << ".vert";
    frag << SHADER_PATH << shaderName << ".frag";

    auto filea = readWholeFile(vert.str());
    auto fileb = readWholeFile(frag.str());
    auto result = LoadShaderProgram(filea, fileb);
    free(filea);
    free(fileb);

    _loadedNamedShaders.insert(std::make_pair(shaderName, result));

    return result;
}
