#include "rendering.h"

class Object {
public:
    int id;
    int x, y;
    int w, h;
    int specialTag;
    Object(int _x, int _y)
    {
        static int globalId;
        id = globalId;
        globalId++;
        x = _x;
        y = _y;
    }

    virtual ~Object() {};

    virtual void draw() {}
};

class SpriteSheet {
public:
    int w, h;
    Texture* sheet;
    SpriteSheet(std::string path)
    {
        char* data = stbi_backend::getImageFromPath(path.c_str(), &w, &h);
        printf("\ngot data from %s %fx%f",path.c_str(), w, h);
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
    std::string spritePath;

    SpriteSheet* sheet;
    int spriteIndex = 0;
    Sprite(int _x, int _y, SpriteSheet* _sheet, int _spriteIndex = 0) : Object(_x, _y) {
        sheet = _sheet;
        specialTag = 0;
        spriteIndex = _spriteIndex;
        printf("\ncreated sprite from sheet");
        w = 32;
        h = 32;
    }

    Sprite(int _x, int _y, std::string _spritePath) : Object(_x, _y) {
        char* data = stbi_backend::getImageFromPath(_spritePath.c_str(), &w, &h);
        spriteTexture = new Texture(w, h, data);
        spritePath = _spritePath;
        printf("\ncreated sprite from texture");
    }

    ~Sprite()
    {
        if (spriteTexture)
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

class Text : public Object {
public:
    Font* f;

    Color color;

    std::string text;
    std::string _fontPath;
    int _size = 0;

    Text(int _x, int _y, std::string fontPath, int size, std::string startText) : Object(_x, _y) {
        specialTag = 1;
        _size = size;
        color = { 255,255,255,1 };
        _fontPath = fontPath;
        f = freetype_backend::createFontFace(fontPath, size);
        text = startText;
    }


    void SetSize(int newSize)
    {
        freetype_backend::removeFontFromCache(f->name);
        delete f;
        _size = newSize;
        f = freetype_backend::createFontFace(_fontPath, newSize);
    }

    void draw() {
        if (!f)
            return;

        Rect dst, src;
        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = _size;
        dst.color = color;

        freetype_backend::drawText(f,dst,text.c_str(), &w);
    }
};