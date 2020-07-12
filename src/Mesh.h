#pragma once

#include "GL/glew.h"

class Mesh {
private:
    GLuint VAO, VBO, IBO;
    unsigned int indexCount;
public:
    Mesh();
    ~Mesh();

    void Create(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void Render();
    void Clear();
};