#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <GL/gl.h>

GLFWwindow* window = nullptr;

class glfw_backend {
public:
    static double mxx, myy;
    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        mxx = xpos;
        myy = ypos;
    }

    static void getMousePosition(double* mx, double* my)
    {
        *mx = mxx;
        *my = myy;
    }

    static bool isMouseDown()
    {
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        return mouseState == GLFW_PRESS ? true : false;
    }

    static bool isMouseReleased()
    {
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        return mouseState == GLFW_RELEASE ? true : false;
    }

    static bool isRightMouseDown()
    {
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        return mouseState == GLFW_PRESS ? true : false;
    }

    static bool isRightMouseReleased()
    {
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        return mouseState == GLFW_RELEASE ? true : false;
    }

    static void createWindow(GLFWkeyfun keyCallback)
    {
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(1280, 720, "OPGLRenderer", NULL, NULL);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        if (!window)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);
    }
};

double glfw_backend::mxx = 0;
double glfw_backend::myy = 0;