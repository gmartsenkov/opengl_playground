#include "Shader.h"

#include <stdio.h>
#include <fstream>
#include <iostream>

Shader::Shader() {
    m_ShaderId = 0;
    m_ProjectionUniform = 0;
    m_ModelUniform = 0;
}

Shader::~Shader() {
    Clear();
}

std::string Shader::ReadFromFile(const char* filePath) {
    std::string fileContent = "";

    std::ifstream shaderFile(filePath);

    if (!shaderFile.is_open()) {
        printf("Could not read shader file %s", filePath);
        return "";
    }

    std::string line = "";
    while (std::getline(shaderFile, line)) {
        fileContent.append(line + "\n");
    }

    return fileContent;
}

void Shader::AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(m_ShaderId, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void Shader::CompileShaders(const char* vertexCode, const char* fragmentCode) {
    m_ShaderId = glCreateProgram();

    if(!m_ShaderId) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(m_ShaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(m_ShaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(m_ShaderId);

    glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(m_ShaderId, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(m_ShaderId);
    glGetProgramiv(m_ShaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(m_ShaderId, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    m_ModelUniform = glGetUniformLocation(m_ShaderId, "model");
    m_ProjectionUniform = glGetUniformLocation(m_ShaderId, "projection");
}

void Shader::Create(const char *vertexCode, const char *fragmentCode) {
    CompileShaders(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char *vertexFilepath, const char *fragmentFilepath) {
    std::string vertexCode = ReadFromFile(vertexFilepath);
    std::string fragmentCode = ReadFromFile(fragmentFilepath);

    CompileShaders(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::Use() {
    glUseProgram(m_ShaderId);
}

void Shader::Clear() {
    if (m_ShaderId != 0) {
        glDeleteProgram(m_ShaderId);
        m_ShaderId = 0;
    }

    m_ModelUniform = 0;
    m_ProjectionUniform = 0;
}

GLuint Shader::GetModelUniform() {
    return m_ModelUniform;
}

GLuint Shader::GetProjectionUniform() {
    return m_ProjectionUniform;
}
