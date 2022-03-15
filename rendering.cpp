#include "rendering.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 projection;

GLuint batch_vao;
GLuint batch_vbo;

Texture* batch_texture;
Shader* batch_shader;

std::vector<GLVertex> batch_buffer;

void Rendering::pushQuad(Rect r, Texture* tex, Shader* shad)
{
    if (batch_texture != tex || batch_shader != shad)
    {
        Rendering::pushBatch();
        tex->use();
        shad->use();
        batch_shader = shad;
        batch_texture = tex;
    }

    // top left
    GLVertex vert;
    vert.x = r.x;
    vert.y = r.y;
    vert.u = 0;
    vert.v = 0;
    //vert.z = r.z;
    vert.r = 255;
    vert.g = 255;
    vert.b = 255;
    vert.a = 1;
    batch_buffer.push_back(vert);
    // bottom left
    GLVertex vert1;
    vert1.x = r.x;
    vert1.y = r.y + r.h;
    vert1.u = 0;
    vert1.v = 1;
    //vert1.z = r.z;
    vert1.r = 255;
    vert1.g = 255;
    vert1.b = 255;
    vert1.a = 1;
    batch_buffer.push_back(vert1);
    // top right
    GLVertex vert2;
    vert2.x = r.x + r.w;
    vert2.y = r.y;
    vert2.u = 1;
    vert2.v = 0;
    //vert2.z = r.z;
    vert2.r = 255;
    vert2.g = 255;
    vert2.b = 255;
    vert2.a = 1;
    batch_buffer.push_back(vert2);
    // bottom right
    GLVertex vert3;
    vert3.x = r.x + r.w;
    vert3.y = r.y + r.h;
    vert3.u = 1;
    vert3.v = 1;
    //vert3.z = r.z;
    vert3.r = 255;
    vert3.g = 255;
    vert3.b = 255;
    vert3.a = 1;
    batch_buffer.push_back(vert3);
    /*
    // z-tl
    GLVertex vert4;
    vert4.x = r.x;
    vert4.y = r.y;
    vert4.z = r.z + r.size;
    vert4.r = 255;
    vert4.g = 255;
    vert4.b = 255;
    vert4.a = 1;
    batch_buffer.push_back(vert4);
    // z-br
    GLVertex vert5;
    vert5.x = r.x;
    vert5.y = r.y + r.h;
    vert5.z = r.z + r.size;
    vert5.r = 255;
    vert5.g = 255;
    vert5.b = 255;
    vert5.a = 1;
    batch_buffer.push_back(vert5);
    // z-tr
    GLVertex vert6;
    vert6.x = r.x + r.w;
    vert6.y = r.y;
    vert6.z = r.z + r.size;
    vert6.r = 255;
    vert6.g = 255;
    vert6.b = 255;
    vert6.a = 1;
    batch_buffer.push_back(vert6);
    // z-br
    GLVertex vert7;
    vert7.x = r.x + r.w;
    vert7.y = r.y + r.h;
    vert7.z = r.z + r.size;
    vert7.r = 255;
    vert7.g = 255;
    vert7.b = 255;
    vert7.a = 1;
    batch_buffer.push_back(vert7);
    */
}

void Rendering::initRendering(Shader* shad)
{
    projection = glm::ortho(0.0f,(float)1280,(float)720, 0.0f, -1.0f, 1.0f);

    shad->use();
    glUniformMatrix4fv(glGetUniformLocation(shad->shaderProgram, "u_projection"), 1, GL_FALSE, &projection[0][0]);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glGenVertexArrays(1, &batch_vao);
    glBindVertexArray(batch_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glGenBuffers(1, &batch_vbo);
}

void Rendering::pushBatch()
{
    if (batch_buffer.size() != 0)
    {
        //printf("\nDrawing %i", batch_buffer.size());

        batch_shader->use();
        batch_texture->use();

        glBindVertexArray(batch_vao);

        glBindBuffer(GL_ARRAY_BUFFER, batch_vbo);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, x));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, u));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, r));
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex) * batch_buffer.size(), batch_buffer.data(), GL_STATIC_DRAW);
        
        glDrawArrays(GL_TRIANGLES, 0, sizeof(batch_buffer));
        batch_buffer.clear();
    }

    batch_texture = NULL;
    batch_shader = NULL;
}