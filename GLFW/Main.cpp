#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include <iostream>

const GLint WIDTH  = 800;
const GLint HEIGHT = 600;

GLuint VAO;      // Vertex array object
GLuint VBO;      // Vertex buffer object
GLuint shader;


// Vertex Shader
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}";

static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    colour = vec4(0.0,1.0,0.0,1.0);                             \n\
}";


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
    GLchar elog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if(!result){
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        std::cout << "Error compiling : " << elog << std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders(){
    shader = glCreateProgram();
    if(!shader){
        std::cout << "Error shader" << std::endl;
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if(!result){
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        std::cout << "Error Linking : " << elog << std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if(!result){
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        std::cout << "Error Validating : " << elog << std::endl;
    }
}


void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);                        
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);
}


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

    CreateTriangle();
    CompileShaders();

    while(!glfwWindowShouldClose(mainwindow))
    {
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES,0,3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainwindow);
    }
}