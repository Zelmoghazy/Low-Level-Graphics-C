#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include <iostream>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main(void)
{
    if(!glfwInit())
    {
        std::cout << "GLFW Initialization failed" << std::endl;        
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

    GLFWwindow *mainwindow  = glfwCreateWindow(WIDTH,
                                               HEIGHT,
                                               "Test",
                                               NULL,
                                               NULL);
    
    if(!mainwindow){
        std::cout << "GLFW Window Creation failed!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    int bufferWidth;
    int bufferHeight;
    glfwGetFramebufferSize(mainwindow,&bufferWidth,&bufferHeight);

    glfwMakeContextCurrent(mainwindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "GLEW Initialization failed!!" << std::endl;
        glfwDestroyWindow(mainwindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0,0,bufferWidth,bufferHeight);

    while(!glfwWindowShouldClose(mainwindow))
    {
        glfwPollEvents();
        glClearColor(0.0f,1.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(mainwindow);
    }
}