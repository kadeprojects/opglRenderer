#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#define DEFINE_callbacks
#include "glfw_backend.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

class Helpers {
public:
    static inline float Align(float value, float size)
    {
        // Added std::abs to give correct behaviour for negative values
        return value - std::abs(std::fmod(value, size));
    }
};