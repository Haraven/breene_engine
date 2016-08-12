#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm\glm.hpp>
#include <map>
#include <vector>
#include "shader_program.h"
#include "texture.h"

namespace breene
{
    namespace text_rendering
    {
        class TextProgram : public ShaderProgram
        {
        public:
            TextProgram();

            virtual breene::text_rendering::TextProgram& Init() override;
            breene::text_rendering::TextProgram& SetOrthographicProjection(const glm::mat4& op);
            breene::text_rendering::TextProgram& SetTextColor(const glm::vec4& color);
            breene::text_rendering::TextProgram& SetTextTextureUnit(GLuint texture_unit);
        };

        class TextureAtlas : public TextureBase
        {
        private:
            static const GLuint CHARSET_SIZE = 128;
            static const GLuint TEX_MAX_WIDTH = 1024;
        public:
            struct Character
            {
                GLfloat advance_x;
                GLfloat advance_y;
                GLfloat bitmap_width;
                GLfloat bitmap_height;
                GLfloat bitmap_left;
                GLfloat bitmap_top;
                GLfloat texture_offset_x;
                GLfloat texture_offset_y;
            };

            TextureAtlas(const FT_Face& face, GLuint size);

            breene::text_rendering::TextureAtlas& Load() override;

            GLuint GetWidth() const { return _width; }
            GLuint GetHeight() const { return _height; }
            const Character* GetChars() const { return _chars; }
        private:
            GLuint _width;
            GLuint _height;
            Character _chars[CHARSET_SIZE];
            FT_Face _face;
        };

        class TextRenderer
        {
        private:
            typedef std::map<std::string, FT_Face> FaceMap;
            typedef std::pair<GLuint, TextureAtlas*> AtlasSizePair;
            typedef std::map<FT_Face, std::vector<AtlasSizePair>> AtlasMap;
            struct Point
            {
                Point(GLfloat x, GLfloat y, GLfloat s, GLfloat t)
                : x(x)
                , y(y)
                , s(s)
                , t(t)
                {}

                GLfloat x;
                GLfloat y;
                GLfloat s;
                GLfloat t;
            };

            void DeallocateResources();
        public:
            TextRenderer();
            breene::text_rendering::TextRenderer& Init(GLuint width, GLuint height);
            breene::text_rendering::TextRenderer& SetTextureUnit(GLuint texture_unit);
            breene::text_rendering::TextRenderer& Render(const std::string& text, TextureAtlas* atlas, const glm::ivec2& pos_2d, GLfloat scale, const glm::vec4& color);

            TextureAtlas* GetAtlas(const GLchar* font, GLuint size);
            ~TextRenderer();
        private:
            GLuint _vbo;
            GLuint _vao;
            FT_Library _freetype_lib;
            TextProgram* _program;
            FaceMap _faces;
            AtlasMap _atlases;
        };
    }
}