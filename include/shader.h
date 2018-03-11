#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

#ifndef SHADER_PATH
#define SHADER_PATH "shaders/"
#endif // SHADER_PATH

GLuint LoadShaderProgram(const std::string &shaderName);

#endif // SHADER_H
