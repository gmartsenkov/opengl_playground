#include "Window.h"

#include <stdio.h>

Window::Window(GLint windowHeight, GLint windowWidth, const char* windowTitle)
    : m_WindowHeight(windowHeight), m_WindowWidth(windowWidth), m_WindowTitle(windowTitle) {

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
