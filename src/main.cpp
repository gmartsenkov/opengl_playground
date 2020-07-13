#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadiant = 3.14159265359 / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

bool direction = true;
float xMoveOffset = 0.0f;
float xMoveStep = 0.007f;
float xRotateAngle = 0.0f;
float xRotateStep = 0.3f;

void CreateTriangle()
{

    unsigned int indices[] = {
            0,3,1,
            1,3,2,
            2,3,0,
            0,1,2
    };

    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh();
    obj1->Create(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->Create(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders()
{
    Shader *shader = new Shader;
    shader->CreateFromFile("shaders/vertex.shader", "shaders/fragment.shader");

    shaderList.push_back(shader);
}

int main() {

    // Init GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    // SETUP GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    Window *mainWindow = new Window(HEIGHT, WIDTH, "Test Window");

    if (!mainWindow->Initialise())
    {
        delete mainWindow;
        return 1;
    }

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed");
        delete mainWindow;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, mainWindow->GetBufferWidth(), mainWindow->GetBufferHeight());

    CreateTriangle();
    CreateShaders();

    // Loop until window closed

    glm::mat4 projection = glm::perspective(45.0f, GLfloat(mainWindow->GetBufferWidth()/mainWindow->GetBufferHeight()), 0.1f, 100.0f);

    while(!mainWindow->GetShouldClose())
    {
        // Get + handle user input events
        glfwPollEvents();

        if (direction)
        {
            xMoveOffset += xMoveStep;
        } else {
            xMoveOffset -= xMoveStep;
        }

        if (abs(xMoveOffset) >= 0.7f)
        {
            direction = !direction;
        }

        if (xRotateAngle > 360.0f) {
            xRotateAngle = 0;
        } else {
            xRotateAngle += xRotateStep;
        }

        // Clear window
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->Use();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, xRotateAngle * toRadiant, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(shaderList[0]->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderList[0]->GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->Render();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-xMoveOffset, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(shaderList[0]->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));

        meshList[1]->Render();

        glUseProgram(0);

        mainWindow->SwapBuffers();
    }

    for(auto shader : shaderList) { delete shader; }
    for(auto mesh : meshList) { delete mesh; }
    delete mainWindow;

    return 0;
}
