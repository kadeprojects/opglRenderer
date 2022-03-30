#include "rendering.h"

class Object {
public:
    int id;
    int x, y;
    int w, h;
    Object(int _x, int _y)
    {
        static int globalId;
        id = globalId;
        globalId++;
        x = _x;
        y = _y;
    }

    virtual void draw() {}
};

class SpriteSheet {
public:
    int w, h;
    Texture* sheet;
    SpriteSheet(char* path)
    {
        char* data = stbi_backend::getImageFromPath(path,&w, &h);
        printf("\ngot data from %s %ix%i",path, w, h);
        sheet = new Texture(w, h, data);
        printf("\ncreated texture");
    }

    Rect returnSrc(int index)
    {
        Rect src;
        src.x = (float)(index * 32) / (float)w;
        src.y = 0;
        src.w = 32.f / (float)w;
        src.h = 32.f / (float)h;
        return src;
    }

    ~SpriteSheet()
    {
        delete sheet;
    }
};

class Sprite : public Object {
public:
    Texture* spriteTexture;

    SpriteSheet* sheet;
    int spriteIndex = 0;
    Sprite(int _x, int _y, SpriteSheet* _sheet, int _spriteIndex = 0) : Object(_x, _y) {
        sheet = _sheet;
        spriteIndex = _spriteIndex;
        printf("\ncreated sprite from sheet");
        w = 32;
        h = 32;
    }

    Sprite(int _x, int _y, char* spritePath) : Object(_x, _y) {
        char* data = stbi_backend::getImageFromPath(spritePath,&w, &h);
        spriteTexture = new Texture(w, h, data);
        printf("\ncreated sprite from texture");
    }

    ~Sprite()
    {
        delete spriteTexture;
    }

    void draw() {
        Rect dst, src;

        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = h;

        if (sheet)
        {
            src = sheet->returnSrc(spriteIndex);
            Rendering::pushQuad(dst,src,sheet->sheet, Rendering::generalShader);
        }
        else
        {
            src.x = 0;
            src.y = 0;
            src.w = 1;
            src.h = 1;
            Rendering::pushQuad(dst, src, spriteTexture, Rendering::generalShader);
        }
    }
};