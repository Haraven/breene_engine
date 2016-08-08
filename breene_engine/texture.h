#pragma once

#include <GL\glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

namespace gl_app
{
    class TextureBase
    {
    protected:
        TextureBase(GLenum texture_target);
    public:
        TextureBase() {}
        virtual TextureBase& Load() = 0;
        bool IsLoaded();
        TextureBase& Bind(GLenum texture_unit);
    protected:
        bool _is_loaded;
        GLuint _txo;
        GLenum _texture_target;
    };

    class Texture2D : public TextureBase
    {
    public:
        Texture2D(const std::string& filename, GLenum texture_target);
        Texture2D& Load() override;
    private:
        std::string _filename;
        GLint _width;
        GLint _height;
        GLint _components;
    };

    class TextureCubeMap : public TextureBase
    {
    public:
        TextureCubeMap(const std::string& posx_file, const std::string& negx_file, const std::string& posy_file, const std::string& negy_file, const std::string& posz_file, const std::string& negz_file);
        ~TextureCubeMap();

        TextureCubeMap& Load() override;
    private:
        std::map<std::string, GLenum> _filenames;
        unsigned char* _pixels;
    };

    class Texture1DRandom : public TextureBase
    {
    public:
        Texture1DRandom(size_t size);

        gl_app::Texture1DRandom& Load() override;

        gl_app::Texture1DRandom& SetSize(size_t size);
        size_t GetSize() { return _size; }
    private:
        size_t _size;
    };

    class TextureCharacter : public TextureBase
    {
    public:
        TextureCharacter(const FT_Face& face);

        gl_app::TextureCharacter& Load() override;
        GLuint GetSizeX() const { return _width; }
        GLuint GetSizeY() const { return _height; }
        GLuint GetBearingLeft() const { return _bearing_left; }
        GLuint GetBearingTop() const { return _bearing_top; }
        GLuint GetAdvance() const { return _advance; }
    private:
        GLuint _width;
        GLuint _height;
        GLuint _bearing_left;
        GLuint _bearing_top;
        GLuint _advance;
        unsigned char* _pixels;
    };
}

