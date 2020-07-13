#pragma once

#include "GLFW/glfw3.h"

class Window {
private:

    GLint m_WindowHeight, m_WindowWidth;
    const char *m_WindowTitle;
    GLint m_BufferHeight, m_BufferWidth;

    GLFWwindow *m_Window;
public:
    Window(GLint windowHeight, GLint windowWidth, const char* windowTitle);
    ~Window();

    bool Initialise();

    GLint GetBufferHeight() const;
    GLint GetBufferWidth() const;
    bool GetShouldClose();

    void SwapBuffers();
};
