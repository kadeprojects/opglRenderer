#include "includes.h"
#include "texture.h"
#include "shader.h"
#include "stbi_backend.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        printf("uh oh");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(1280, 720, "OPGLRenderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);


    // create shaders
    
    char* vert = "#version 150 core" 
    "in vec3 v_position;"
    "in vec2 v_uv;"
    "in vec4 v_colour;"
    "out vec2 f_uv;"
    "out vec4 f_colour;"
    "uniform mat4 u_projection;"
    "void main()"
    "{"
        "f_uv = v_uv;"
        "f_colour = v_colour;"
        "gl_Position = u_projection * vec4(v_position.xyz, 0.0, 1.0);"
    "}";

    char* frag = "#version 150 core" 
    "uniform sampler2D u_texture;"
    "in vec2 f_uv;"
    "in vec4 f_colour;"
    "out vec4 o_colour;"
    "void main()"
    "{"
        "o_colour = texture (u_texture, f_uv) * f_colour;"
        "if (o_colour.a == 0.0)"
            "discard;"
    "}";

    Shader* shad = new Shader(vert,frag);
    shad->use();

    int w,h;

    char* data = stbi_backend::getImageFromPath("test.png",&w,&h);

    Texture* testText = new Texture(w,h,data);

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT);

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}