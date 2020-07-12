cmake_minimum_required(VERSION 3.16)
project(OpenGLCourse)

set(CMAKE_CXX_STANDARD 14)

include_directories(dependencies/glew-2.1.0/include)
include_directories(dependencies/glfw-3.3.2.bin.WIN32/include)
include_directories(dependencies/glm)

link_directories(dependencies/glew-2.1.0/lib/Release/Win32)
link_directories(dependencies/glfw-3.3.2.bin.WIN32/lib-vc2019)

file(GLOB OpenGLCourseSrc "src/*")

file(COPY dependencies/glew-2.1.0/bin/Release/Win32/glew32.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(OpenGLCourse ${OpenGLCourseSrc})
target_link_libraries(OpenGLCourse opengl32.lib glew32.lib glfw3.lib)