#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <GL/gl.h>


typedef void(__cdecl *keyPress)(int key,int action, int mods);

struct keyCallback {
    keyPress call;
    int id = 0;
};

std::vector<keyCallback> callbacks = {};
int lastId = 0;
GLFWwindow* window = nullptr;

class KeyboardManager {
public:
    
    static int registerCallback(keyPress call)
    {
        keyCallback callback;
        callback.call = call;
        callback.id = lastId + 1;
        lastId++;
        callbacks.push_back(callback);
        return callback.id;
    }

    static void removeCallback(int id)
    {
        for(int i = 0; i < callbacks.size(); i++)
        {
            if (callbacks[i].id == id)
            {
                callbacks.erase(callbacks.begin() + i);
                break;
            }
        }
    }

    static void removeAllCallbacks()
    {
        callbacks.clear();
    }

    static void disperseCallbacks(int key, int action, int mods)
    {
        for(keyCallback& call : callbacks)
            call.call(key, action, mods);
    }
};

class glfw_backend {
public:

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        printf("\nyo");
        KeyboardManager::disperseCallbacks(key,action,mods);
    }
    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void createWindow()
    {
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwSetErrorCallback(error_callback);
        window = glfwCreateWindow(1280, 720, "OPGLRenderer", NULL, NULL);
        glfwSetKeyCallback(window, key_callback);
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