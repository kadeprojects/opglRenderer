#pragma once
#include "stbi_backend.h"
#include "includes.h"

class Texture {
public:
    GLuint textureID;

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

    }

    Texture(int width, int height, unsigned char* data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);

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

    }

    void use()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
};

class Shader {
public:
    GLuint shaderProgram;

    Shader(char* vert, char* frag)
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

    Font(FT_Face face)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
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
                face->glyph->advance.x
            };
            Characters[c] = character;
        }

        FT_Done_Face(face);
    }
};

std::unordered_map<char*, Font*> ft_fontFaces;

class freetype_backend {
public:
    static void initFreeType()
    {
        if (FT_Init_FreeType(&ft))
            printf("\nFailure to init FreeType");
        else
            printf("\nSuccessfully initialized FreeType");
    }

    static Font* createFontFace(char* path, int size)
    {
        FT_Face face;
        if (FT_New_Face(ft, path, 0, &face))
            printf("\nFailed to load font %s", path);
        else
        {
            FT_Set_Pixel_Sizes(face, 0, size);  
            return new Font(face);
        }
    }

    static void drawText(Font* f, Rect startRect, char* text)
    {
        Rect srcRect;
        srcRect.w = 1;
        srcRect.h = 1;
        for(char c : std::string(text))
        {
            Character ch = f->Characters[c];
            Rect characterRect;
            characterRect.x = startRect.x + ch.Bearing.x;
            characterRect.y = startRect.y - (ch.Size.y - ch.Bearing.y);
            characterRect.w = ch.Size.x;
            characterRect.h = ch.Size.y;
            startRect.x += (ch.Advance >> 6);
            Rendering::pushQuad(characterRect, srcRect, ch.tex, Rendering::textShader);
            Rendering::pushBatch();
        }
        Rendering::pushBatch();
    }
};