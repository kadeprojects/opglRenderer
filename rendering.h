#pragma once
#include "stbi_backend.h"
#include "includes.h"

class Texture {
public:
    GLuint textureID;

    Texture(int width, int height, char* data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);
        
		//Use the given buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    void use()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
};

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

struct Rect {
    // cordinates
    float x,y;
    // width, height, depth
    float w, h, size;
};
struct GLVertex {
    float x, y;
    float u, v;
    float r, g, b;
    float a;
};

class Rendering {
public:
    static Shader* generalShader;

    static void initRendering(Shader* shad);
    static void pushQuad(Rect r, Rect src, Texture* tex, Shader* shad);
    static void pushBatch();
};