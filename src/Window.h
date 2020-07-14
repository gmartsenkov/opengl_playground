#pragma once

#include "GLFW/glfw3.h"

class Window {
private:

    GLint m_WindowHeight, m_WindowWidth;
    const char *m_WindowTitle;
    GLint m_BufferHeight, m_BufferWidth;

    bool keys[1024];

    GLfloat m_LastX, m_LastY;
    GLfloat m_Mouse_X_Change, m_Mouse_Y_Change;
    bool m_MouseFirstMoved = true;

    GLFWwindow *m_Window;
public:
    Window(GLint windowHeight, GLint windowWidth, const char* windowTitle);
    ~Window();

    bool Initialise();

    GLint GetBufferHeight() const;
    GLint GetBufferWidth() const;
    bool GetShouldClose();

    void SwapBuffers();

    bool* GetKeys();
    GLfloat GetMouse_X_Change();
    GLfloat GetMouse_Y_Change();

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
};
