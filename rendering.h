#pragma once
#include "stbi_backend.h"
#include "includes.h"

class Texture {
public:
    GLuint textureID;

    unsigned char* _data;

    Texture(int width, int height, char* data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);
        
		//Use the given buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
        _data = (unsigned char*)data;

    }

    Texture(int width, int height, unsigned char* data)
    {
        _data = data;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED ,
            width,
            height,
            0,
            GL_RED ,
            GL_UNSIGNED_BYTE,
            data
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }

    void use()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
};

class Shader {
public:
    GLuint shaderProgram;

    Shader(const char* vert, const char* frag)
    {
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vert, NULL);
        glCompileShader(vertex);



        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &frag, NULL);
        glCompileShader(fragment);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);

        glBindAttribLocation(shaderProgram, 0, "v_position");
        glBindAttribLocation(shaderProgram, 1, "v_uv");
        glBindAttribLocation(shaderProgram, 2, "v_color");

        glLinkProgram(shaderProgram);

        
        GLint log_length;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &log_length);
			
		if (log_length > 1)
		{
			std::string info_log(log_length, 0);
			printf("\nProgram Error: &s", info_log.c_str());
		}
        
        printf("\nSuccessfully compiled shader!");
    }

    void use() 
    { 
        glUseProgram(shaderProgram); 
    }
};

struct Rect {
    // cordinates
    float x = 0,y = 0;
    // width, height, depth
    float w = 0, h = 0, size = 0;
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
    static Shader* textShader;

    static void initRendering(Shader* shad);
    static void pushQuad(Rect r, Rect src, Texture* tex, Shader* shad);
    static void pushBatch();
};


FT_Library ft;

struct Character {
    Texture* tex;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class Font {
public:
    std::unordered_map<char, Character> Characters;

    std::string name;

    Font(FT_Face face)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        name = face->style_name;
  
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                printf("\nFailed to load Glyph");
                continue;
            }
            Texture* texture = new Texture(face->glyph->bitmap.width,face->glyph->bitmap.rows,face->glyph->bitmap.buffer);
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters[c] = character;
        }

        FT_Done_Face(face);
    }

    ~Font()
    {
        for (unsigned char c = 0; c < 128; c++)
        {
            delete Characters[c].tex;
        }
        Characters.clear();
    }
};

std::unordered_map<std::string, Font*> ft_fontFaces;

class freetype_backend {
public:
    static void initFreeType()
    {
        if (FT_Init_FreeType(&ft))
            printf("\nFailure to init FreeType");
        else
            printf("\nSuccessfully initialized FreeType");
    }

    static void clearFontCache()
    {
        for (auto const& x : ft_fontFaces)
        {
            delete x.second;
        }
        ft_fontFaces.clear();
    }

    static Font* createFontFace(std::string path, int size)
    {
        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face))
        {
            printf("\nFailed to load font %s", path.c_str());
            FT_Done_Face(face);
        }
        else
        {
            std::string name = face->family_name + std::to_string(size);

            if (ft_fontFaces[name] != nullptr)
            {
                FT_Done_Face(face);
                return ft_fontFaces[name];
            }

            FT_Set_Pixel_Sizes(face, 0, size);
            ft_fontFaces[name] = new Font(face);
            return ft_fontFaces[name];
        }
    }

    static void drawText(Font* f, Rect startRect, const char* text)
    {
        if (f->Characters.size() == 0)
            return;
        Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = 1;
        srcRect.h = 1;
        for(char c : std::string(text))
        {
            Character ch = f->Characters[c];
            Rect characterRect;
            characterRect.x = startRect.x + ch.Bearing.x;
            characterRect.y = startRect.y - ch.Bearing.y;
            characterRect.w = ch.Size.x;
            characterRect.h = ch.Size.y;
            startRect.x += (ch.Advance >> 6);
            Rendering::pushQuad(characterRect, srcRect, ch.tex, Rendering::textShader);
            Rendering::pushBatch();
        }
        Rendering::pushBatch();
    }
};