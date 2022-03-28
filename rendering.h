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
        float x, y;
        float u, v;
	    float r, g, b;
        float a;
    };

class Rendering {
public:
    static Shader* generalShader;

    static void initRendering(Shader* shad);
    static void pushQuad(Rect r, Rect src, Texture* tex, Shader* shad);
    static void pushBatch();
};