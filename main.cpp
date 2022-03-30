#include "menu.h"

char *generic_shader_vert = "\
#version 150 core\n\
in vec2 v_position;\
in vec2 v_uv;\
in vec4 v_colour;\
out vec2 f_uv;\
out vec4 f_colour;\
uniform mat4 u_projection;\
void main()\
{\
f_uv = v_uv;\
f_colour = v_colour;\
gl_Position = u_projection * vec4(v_position.xy, 0.0, 1.0);\
}";
char *generic_shader_frag = "\
#version 150 core\n\
uniform sampler2D u_texture;\
in vec2 f_uv;\
in vec4 f_colour;\
out vec4 o_colour;\
void main()\
{\
o_colour = texture(u_texture, f_uv) * f_colour;\
if (o_colour.a == 0.0)\
{\
discard;\
}\
}";



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    glfw_backend::createWindow();

    // create shaders

    Rendering::generalShader = new Shader(generic_shader_vert,generic_shader_frag);
    Rendering::generalShader->use();

    Rendering::initRendering(Rendering::generalShader);

    MenuManager::switchMenu(new Gameplay(), false);

    printf("\nhahah");

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1,.5,.5,1);

        MenuManager::currentMenu->update();

        MenuManager::currentMenu->draw();

        Rendering::pushBatch();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}



// rendering.cpp outputted here because of header hell

glm::mat4 projection;

GLuint batch_vao;
GLuint batch_vbo;

Texture* batch_texture;
Shader* batch_shader;

Shader* Rendering::generalShader = nullptr;

std::vector<GLVertex> batch_buffer;

void Rendering::pushQuad(Rect r, Rect src, Texture* tex, Shader* shad)
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
    vert.u = src.x;
    vert.v = src.y;
    //vert.z = r.z;
    vert.r = 1;
    vert.g = 1;
    vert.b = 1;
    vert.a = 1;
    // bottom left
    GLVertex vert1;
    vert1.x = r.x;
    vert1.y = r.y + r.h;
    vert1.u = src.x;
    vert1.v = src.y + src.h;
    //vert1.z = r.z;
    vert1.r = 1;
    vert1.g = 1;
    vert1.b = 1;
    vert1.a = 1;
    // top right
    GLVertex vert2;
    vert2.x = r.x + r.w;
    vert2.y = r.y;
    vert2.u = src.x + src.w;
    vert2.v = src.y;
    //vert2.z = r.z;
    vert2.r = 1;
    vert2.g = 1;
    vert2.b = 1;
    vert2.a = 1;
    // bottom right
    GLVertex vert3;
    vert3.x = r.x + r.w;
    vert3.y = r.y + r.h;
    vert3.u = src.x + src.w;
    vert3.v = src.y + src.h;
    //vert3.z = r.z;
    vert3.r = 1;
    vert3.g = 1;
    vert3.b = 1;
    vert3.a = 1;
    batch_buffer.push_back(vert);
    batch_buffer.push_back(vert1);
    batch_buffer.push_back(vert2);
    batch_buffer.push_back(vert2);
    batch_buffer.push_back(vert1);
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
        
        glDrawArrays(GL_TRIANGLES, 0, batch_buffer.size());
        batch_buffer.clear();
    }

    batch_texture = NULL;
    batch_shader = NULL;
}