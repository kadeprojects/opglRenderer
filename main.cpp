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

    GLFWwindow* window = glfw_backend::window;

    // create shaders

    Rendering::generalShader = new Shader(generic_shader_vert,generic_shader_frag);
    Rendering::generalShader->use();

    Rendering::initRendering(Rendering::generalShader);

    MenuManager::switchMenu(Gameplay(), false);

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1,.5,.5,1);

        MenuManager::currentMenu.update();
        MenuManager::currentMenu.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}