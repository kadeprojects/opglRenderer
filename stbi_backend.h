#pragma once

class stbi_backend {
public:
    static char* getImageFromPath(const char* path,int* w,int* h);
};