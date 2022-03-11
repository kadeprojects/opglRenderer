#include "includes.h"
#include "shader.h"
#include "texture.h"

    struct Rect {
        // cordinates
        float x,y;
        // width, height, depth
        float w, h, size;
    };
    struct GLVertex {
        float x, y, z;
        float u, v;
	    float r, g, b, a;
    };

class Rendering {
public:
    static void initRendering(Shader* shad);
    static void pushQuad(Rect r, Texture* tex, Shader* shad);
    static void pushBatch();
};