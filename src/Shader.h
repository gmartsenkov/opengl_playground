#pragma once

#include <string>
#include "GL/glew.h"

class Shader {
private:
    GLuint m_ShaderId, m_ModelUniform, m_ProjectionUniform;

    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
    void CompileShaders(const char* vertexCode, const char* fragmentCode);
    std::string ReadFromFile(const char* filePath);
public:
    Shader();
    ~Shader();

    void Create(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char* vertexFilepath, const char* fragmentFilepath);
    void Use();
    void Clear();

    GLuint GetModelUniform();
    GLuint GetProjectionUniform();
};
