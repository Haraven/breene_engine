#include "text_rendering.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include "my_constants.h"
#include "utils.h"

breene::text_rendering::TextProgram::TextProgram()
: ShaderProgram()
{}

breene::text_rendering::TextProgram & breene::text_rendering::TextProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(FONT_VERT_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(FONT_FRAG_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::text_rendering::TextProgram & breene::text_rendering::TextProgram::SetOrthographicProjection(const glm::mat4 & op)
{
    GLuint ortho_loc = GetUniformLocation(OP_UNIFORM);

    glUniformMatrix4fv(ortho_loc, 1, GL_FALSE, glm::value_ptr(op));
    
    return *this;
}

breene::text_rendering::TextProgram & breene::text_rendering::TextProgram::SetTextColor(const glm::vec4 & color)
{
    GLuint color_loc = GetUniformLocation(COLOR_UNIFORM);

    glUniform4f(color_loc, color.r, color.g, color.b, color.a);

    return *this;
}

breene::text_rendering::TextProgram & breene::text_rendering::TextProgram::SetTextTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint texture_unit_loc = GetUniformLocation(TEXTMAP_UNIFORM);

    glUniform1i(texture_unit_loc, texture_unit);

    return *this;
}

breene::text_rendering::TextureAtlas * breene::text_rendering::TextRenderer::GetAtlas(const GLchar * font, GLuint size)
{
    TextureAtlas* atlas = nullptr;
    FaceMap::iterator it = _faces.find(font);
    if (it != _faces.end())
    {
        FT_Face face = it->second;
        AtlasMap::iterator it2 = _atlases.find(face);
        if (it2 != _atlases.end())
        {
            std::vector<AtlasSizePair> atlases = it2->second;
            for (std::vector<AtlasSizePair>::iterator it = atlases.begin(); it != atlases.end(); ++it)
            {
                if (it->first == size)
                    atlas = it->second;
            }
            if (atlas == nullptr)
            {
                atlas = new TextureAtlas(face, size);
                atlas->Load();
                it2->second.push_back(AtlasSizePair(size, atlas));
            }
        }
    }
    else
    {
        FT_Face face;
        _faces.insert(std::pair<std::string, FT_Face>(font, face));
        it = _faces.find(font);
        if (FT_New_Face(_freetype_lib, font, 0, &(it->second)) != NULL)
        {
            _faces.erase(it);
            throw std::runtime_error(std::string("Error creating new face for font \"") + font + "\"");
        }
        atlas = new TextureAtlas(it->second, size);
        atlas->Load();
        AtlasMap::iterator it2 = _atlases.find(it->second);
        if (it2 != _atlases.end())
            it2->second.push_back(AtlasSizePair(size, atlas));
        else
        {
            std::vector<AtlasSizePair> tmp;
            tmp.push_back(AtlasSizePair(size, atlas));
            _atlases.insert(std::pair<FT_Face, std::vector<AtlasSizePair>>(it->second, tmp));
        }
    }

    return atlas;
}

void breene::text_rendering::TextRenderer::DeallocateResources()
{
    std::for_each(_atlases.begin(), _atlases.end(), [](std::pair<FT_Face, std::vector<AtlasSizePair>> entry)
    {
        FT_Done_Face(entry.first);
        std::for_each(entry.second.begin(), entry.second.end(), [](std::pair<GLuint, TextureAtlas*> pair)
        {
            delete pair.second;
            pair.second = nullptr;
        });
        entry.second.clear();
    });
    _atlases.clear();
    _faces.clear();

    if (_vao != INVALID_VAL)
    {
        glDeleteBuffers(1, &_vao);
        _vao = INVALID_VAL;
    }
}

breene::text_rendering::TextRenderer::TextRenderer()
: _program(nullptr)
, _vao(INVALID_VAL)
, _vbo(INVALID_VAL)
{}

breene::text_rendering::TextRenderer & breene::text_rendering::TextRenderer::Init(GLuint width, GLuint height)
{
    if (FT_Init_FreeType(&_freetype_lib) != NULL) throw std::runtime_error("Error initializing text renderer. Error caused by FT_Init_FreeType()");
    
    _program = new TextProgram();
    _program->Init().Use();
    _program->SetOrthographicProjection(glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height)));

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(0);

    return *this;
}

breene::text_rendering::TextRenderer & breene::text_rendering::TextRenderer::SetTextureUnit(GLuint texture_unit)
{
    _program->Use();
    _program->SetTextTextureUnit(texture_unit);

    return *this;
}

breene::text_rendering::TextRenderer & breene::text_rendering::TextRenderer::Render(const std::string & text, TextureAtlas* atlas, const glm::ivec2 & pos_2d, GLfloat scale, const glm::vec4 & color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _program->Use();
    _program->SetTextColor(color);
    atlas->Bind(TEXT_TEXTURE_UNIT);

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    std::vector<Point> coords;
    GLint x = pos_2d.x;
    GLint y = pos_2d.y;
    const TextureAtlas::Character* chars = atlas->GetChars();
    for (std::string::const_iterator ch = text.cbegin(); ch != text.cend(); ++ch)
    {
        const TextureAtlas::Character& character = chars[*ch];
        GLfloat x_pos = x + character.bitmap_left * scale;
        GLfloat y_pos = -y - character.bitmap_top * scale;
        GLfloat width = character.bitmap_width * scale;
        GLfloat height = character.bitmap_height * scale;

        x += static_cast<GLint>(character.advance_x * scale);
        y += static_cast<GLint>(character.advance_y * scale);

        if (width == 0 || height == 0)
            continue;

        coords.push_back
        (
            Point
            (
                x_pos,
                -y_pos - height,
                character.texture_offset_x,
                character.texture_offset_y + character.bitmap_height / atlas->GetHeight()
            )
        );
        coords.push_back
        (
            Point
            (
                x_pos,
                -y_pos, 
                character.texture_offset_x,
                character.texture_offset_y
            )
        );
        coords.push_back
        (
            Point
            (
                x_pos + width,
                -y_pos,
                character.texture_offset_x + character.bitmap_width / atlas->GetWidth(),
                character.texture_offset_y
            )
        );
        coords.push_back
        (
            Point
            (
                x_pos,
                -y_pos - height,
                character.texture_offset_x,
                character.texture_offset_y + character.bitmap_height / atlas->GetHeight()
            )
        );
        coords.push_back
        (
            Point
            (
                x_pos + width,
                -y_pos,
                character.texture_offset_x + character.bitmap_width / atlas->GetWidth(), 
                character.texture_offset_y
            )
        );
        coords.push_back
        (
            Point
            (
                x_pos + width,
                -y_pos - height,
                character.texture_offset_x + character.bitmap_width / atlas->GetWidth(),
                character.texture_offset_y + character.bitmap_height / atlas->GetHeight()
            )
        );
    }

    GLuint size = ConvertToGLuint(coords.size());
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(Point), coords.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, size);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    return *this;
}

//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::Init(GLuint width, GLuint height)
//{
//    if (_program == nullptr)
//        _program = new TextProgram();
//    _program->Init().Use();
//    _program->SetOrthographicProjection(glm::ortho(0.0f, static_cast<GLfloat>(_width), 0.0f, static_cast<GLfloat>(_height)));
//    if (FT_Init_FreeType(&_freetype_lib) != NULL) throw std::runtime_error("Error initializing FreeType library");
//    SetFontFace("verdana.ttf");
//    
//    _width = width;
//    _height = height;
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::SetFontFace(const std::string & font, const GLuint size)
//{
//    //FT_Done_Face(_face);
//    FT_New_Face(_freetype_lib, font.c_str(), 0, &_face);
//
//    FT_Set_Pixel_Sizes(_face, 0, size);
//
//    _font = "verdana.ttf";
//    _is_loaded = false;
//
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::SetFontSize(const GLuint size)
//{
//    //SetFontFace(_font, size);
//
//    //_is_loaded = false;
//
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::SetTextureUnit(GLuint texture_unit)
//{
//    _program->SetTextTextureUnit(texture_unit);
//
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::Render(const std::string& text, const glm::vec2& pos_2d, GLfloat scale, const glm::vec4& color)
//{
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    if (!_is_loaded)
//        Load();
//    _program->Use();
//    _program->SetTextColor(color);
//
//    glBindVertexArray(_vao);
//    GLfloat x = pos_2d.x;
//    GLfloat y = pos_2d.y;
//    for (std::string::const_iterator ch = text.cbegin(); ch != text.cend(); ++ch)
//    {
//        TextureCharacter* ch_tex = _char_mappings.at(*ch);
//        GLfloat x_pos = x + ch_tex->GetBearingLeft() * scale;
//        GLfloat y_pos = -y - ch_tex->GetBearingTop() * scale;
//        GLfloat width = ch_tex->GetSizeX() * scale;
//        GLfloat height = ch_tex->GetSizeY() * scale;
//
//        TextVertex vertices[] =
//        {
//            TextVertex(glm::vec2(x_pos, -y_pos), glm::vec2(0.0f, 0.0f)), 
//            TextVertex(glm::vec2(x_pos + width, -y_pos), glm::vec2(1.0f, 0.0f)), 
//            TextVertex(glm::vec2(x_pos, -y_pos - height), glm::vec2(0.0f, 1.0f)),
//            TextVertex(glm::vec2(x_pos + width, -y_pos - height), glm::vec2(1.0f, 1.0f))
//        };
//
//        ch_tex->Bind(TEXT_TEXTURE_UNIT);
//        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//        x += (ch_tex->GetAdvance() >> 6) * scale;
//    }
//
//    glBindVertexArray(0);
//    glDisable(GL_BLEND);
//
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::GenerateChars()
//{
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//    for (GLubyte ch = 0; ch < 128; ++ch)
//    {
//        if (FT_Load_Char(_face, ch, FT_LOAD_RENDER) != NULL)
//        {
//            std::cerr << "Error loading character \"" << ch << "\"" << std::endl;
//            continue;
//        }
//        TextureCharacter* my_char = new TextureCharacter(_face);
//        my_char->Load();
//        _char_mappings.insert(std::pair<GLchar, TextureCharacter*>(ch, my_char));
//    }
//
//    FT_Done_Face(_face);
//    FT_Done_FreeType(_freetype_lib);
//
//    return *this;
//}
//
//gl_app::text_rendering::TextRenderer & gl_app::text_rendering::TextRenderer::Load()
//{
//    if (_char_mappings.size() > 0)
//    {
//        std::for_each(_char_mappings.begin(), _char_mappings.end(), [](std::pair<GLchar, TextureCharacter*> pair)
//        {
//            if (pair.second != nullptr)
//            {
//                delete pair.second;
//                pair.second = nullptr;
//            }
//        });
//        _char_mappings.clear();
//    }
//
//    GenerateChars();
//
//    glGenVertexArrays(1, &_vao);
//    glGenBuffers(1, &_vbo);
//
//    glBindVertexArray(_vao);
//    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
//    
//    glEnableVertexAttribArray(0);
//    
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    
//    glBindVertexArray(0);
//
//    _is_loaded = true;
//
//    return *this;
//}

breene::text_rendering::TextRenderer::~TextRenderer()
{
    DeallocateResources();
}

breene::text_rendering::TextureAtlas::TextureAtlas(const FT_Face & face, GLuint size)
: TextureBase(GL_TEXTURE_2D)
{
    _width = 0;
    _height = 0;
    _face = face;

    FT_Set_Pixel_Sizes(_face, 0, size);
    
    memset(_chars, NULL, CHARSET_SIZE * sizeof(Character));
}

breene::text_rendering::TextureAtlas & breene::text_rendering::TextureAtlas::Load()
{
    FT_GlyphSlot glyph = _face->glyph;

    GLuint row_width = 0;
    GLuint row_height = 0;
    for (GLuint ch = 0; ch < 128; ++ch)
    {
        if (FT_Load_Char(_face, ch, FT_LOAD_RENDER) != NULL)
        {
            std::cerr << "Error loading character \"" << static_cast<GLchar>(ch) << "\"" << std::endl;
            continue;
        }

        if (row_width + glyph->bitmap.width + 1 >= TEX_MAX_WIDTH)
        {
            _width = std::max(_width, row_width);
            _height += row_height;
            row_width = row_height = 0;
        }

        row_width += glyph->bitmap.width + 1;
        row_height = std::max(row_height, glyph->bitmap.rows);
    }

    _width = std::max(_width, row_width);
    _height += row_height;

    glGenTextures(1, &_txo);
    Bind(TEXT_TEXTURE_UNIT);
    // _program->SetTextTextureUnit(TEXT_TEXTURE_UNIT_INDEX) needs to be called before this
    glTexImage2D(_texture_target, 0, GL_RED, _width, _height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint offset_x = 0;
    GLint offset_y = 0;

    row_height = 0;

    for (GLuint ch = 0; ch < 128; ++ch)
    {
        if (FT_Load_Char(_face, ch, FT_LOAD_RENDER) != NULL)
        {
            std::cerr << "Error loading character \"" << static_cast<GLchar>(ch) << "\"" << std::endl;
            continue;
        }

        if (offset_x + glyph->bitmap.width + 1 >= TEX_MAX_WIDTH)
        {
            offset_y += row_height;
            row_height = offset_x = 0;
        }

        glTexSubImage2D(_texture_target, 0, offset_x, offset_y, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
        _chars[ch].advance_x = static_cast<GLfloat>(glyph->advance.x >> 6);
        _chars[ch].advance_y = static_cast<GLfloat>(glyph->advance.y >> 6);

        _chars[ch].bitmap_width = static_cast<GLfloat>(glyph->bitmap.width);
        _chars[ch].bitmap_height = static_cast<GLfloat>(glyph->bitmap.rows);

        _chars[ch].bitmap_left = static_cast<GLfloat>(glyph->bitmap_left);
        _chars[ch].bitmap_top = static_cast<GLfloat>(glyph->bitmap_top);

        _chars[ch].texture_offset_x = offset_x / static_cast<GLfloat>(_width);
        _chars[ch].texture_offset_y = (offset_y + 0.23f) / static_cast<GLfloat>(_height);

        row_height = std::max(row_height, glyph->bitmap.rows);
        offset_x += glyph->bitmap.width + 1;
    }
    
    _is_loaded = true;

    return *this;
}
