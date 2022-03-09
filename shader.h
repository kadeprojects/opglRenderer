#include "includes.h"

class Shader {
public:
    GLuint shaderProgram;

    Shader(char* vert, char* frag)
    {
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vert, NULL);
        glCompileShader(vertex);
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &frag, NULL);
        glCompileShader(fragment);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);
        glLinkProgram(shaderProgram);
        
        printf("Successfully compiled shader!");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() 
    { 
        glUseProgram(shaderProgram); 
    }
};