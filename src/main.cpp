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

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadiant = 3.14159265359 / 180.0f;

std::vector<Mesh*> meshList;

GLuint VAO, VBO, IBO, shader, modelUniform, projectionUniform;
bool direction = true;
float xMoveOffset = 0.0f;
float xMoveStep = 0.007f;
float xRotateAngle = 0.0f;
float xRotateStep = 0.3f;

// Vertex shader
static const char* vShader = "\n"
                             "#version 330 \n"
                             "layout (location = 0) in vec3 pos;\n"
                             "out vec4 vCol;\n"
                             "uniform mat4 model;\n"
                             "uniform mat4 projection;\n"
                             "void main()\n"
                             "{\n"
                             "gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);\n"
                             "vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n"
                             "}";
static const char* fShader = "\n"
                             "#version 330 \n"
                             "in vec4 vCol;\n"
                             "out vec4 colour;\n"
                             "void main()\n"
                             "{\n"
                             "colour = vCol;\n"
                             "}";

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

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
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
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if(!shader) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    modelUniform = glGetUniformLocation(shader, "model");
    projectionUniform = glGetUniformLocation(shader, "projection");
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

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if(!mainWindow)
    {
        printf("GLFW window creation failed");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::perspective(45.0f, GLfloat(bufferWidth/bufferHeight), 0.1f, 100.0f);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop until window closed

    while(!glfwWindowShouldClose(mainWindow))
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

        glUseProgram(shader);


        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, xRotateAngle * toRadiant, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->Render();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-xMoveOffset, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

        meshList[1]->Render();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
