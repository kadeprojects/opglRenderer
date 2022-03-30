#pragma once
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

        glBindAttribLocation(shaderProgram, 0, "v_position");
        glBindAttribLocation(shaderProgram, 1, "v_uv");
        glBindAttribLocation(shaderProgram, 2, "v_color");

        glLinkProgram(shaderProgram);
        
        printf("\nSuccessfully compiled shader!");
    }

    void use() 
    { 
        glUseProgram(shaderProgram); 
    }
};