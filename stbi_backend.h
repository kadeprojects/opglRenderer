#include "includes.h"
class stbi_backend {
public:
    static char* getImageFromPath(char* path,int* w,int* h);
};