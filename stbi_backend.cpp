#include "stbi_backend.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

char* stbi_backend::getImageFromPath(const char* path, int* width, int* height)
{
    int n = 4;
    char* data = (char*) stbi_load(path, width, height, &n, 0);
    return data;
}