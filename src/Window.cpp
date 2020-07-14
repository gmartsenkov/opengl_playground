#include "Window.h"

#include <stdio.h>

Window::Window(GLint windowHeight, GLint windowWidth, const char* windowTitle)
    : m_WindowHeight(windowHeight), m_WindowWidth(windowWidth), m_WindowTitle(windowTitle) {

    m_Mouse_X_Change = 0.0f;
    m_Mouse_Y_Change = 0.0f;

    for(int i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::Initialise() {
    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle, NULL, NULL);
    if(!m_Window)
    {
        printf("GLFW window creation failed");
        glfwTerminate();
        return false;
    }

    glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);


    // Set context for GLEW to use
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, KeyCallback);
    glfwSetCursorPosCallback(m_Window, MouseCallback);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

GLint Window::GetBufferHeight() const {
    return m_WindowHeight;
}

GLint Window::GetBufferWidth() const {
    return m_BufferWidth;
}

bool Window::GetShouldClose() {
    return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::KeyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    Window *thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key < 0 || key > 1023) { return; }

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(thisWindow->m_Window, GL_TRUE);
        return;
    }

    if (action == GLFW_PRESS) {
        thisWindow->keys[key] = true;
    }
    if (action == GLFW_RELEASE) {
        thisWindow->keys[key] = false;
    }
}

void Window::MouseCallback(GLFWwindow *window, double xPos, double yPos) {
    Window *thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (thisWindow->m_MouseFirstMoved) {
        thisWindow->m_LastX = xPos;
        thisWindow->m_LastY = yPos;

        thisWindow->m_MouseFirstMoved = false;
    }

    thisWindow->m_Mouse_X_Change = xPos - thisWindow->m_LastX;
    thisWindow->m_Mouse_Y_Change = thisWindow->m_LastY - yPos;

    thisWindow->m_LastX = xPos;
    thisWindow->m_LastY = yPos;
}

bool *Window::GetKeys() {
    return keys;
}

GLfloat Window::GetMouse_X_Change() {
    GLfloat theChange = m_Mouse_X_Change;
    m_Mouse_X_Change = 0;
    return theChange;
}

GLfloat Window::GetMouse_Y_Change() {7
    GLfloat theChange = m_Mouse_Y_Change;
    m_Mouse_Y_Change = 0;
    return theChange;
}
