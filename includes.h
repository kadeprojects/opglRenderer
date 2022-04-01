#include <stdio.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#define DEFINE_callbacks
#include "glfw_backend.h"

class Helpers {
public:
    static inline float Align(float value, float size)
    {
        // Added std::abs to give correct behaviour for negative values
        return value - std::abs(std::fmod(value, size));
    }
};