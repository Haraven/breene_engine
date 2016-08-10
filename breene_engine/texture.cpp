#include "texture.h"
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

#include "my_constants.h"
#include "utils.h"

gl_app::Texture2D::Texture2D(const std::string & filename, GLenum texture_target)
: gl_app::TextureBase(texture_target)
{
    _filename = filename;
}

gl_app::Texture2D& gl_app::Texture2D::Load()
{
    unsigned char* pixels = stbi_load(_filename.c_str(), &_width, &_height, &_components, STBI_rgb_alpha);
    if (pixels == nullptr) throw std::runtime_error("STBI could not load pixel information for image \"" + _filename + "\"");

    glGenTextures(1, &_txo);
    glBindTexture(_texture_target, _txo);
    glTexImage2D(_texture_target, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameterf(_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _is_loaded = true;

    delete[] pixels;
    pixels = nullptr;

    glBindTexture(_texture_target, 0);

    return *this;
}

gl_app::TextureBase::TextureBase(GLenum texture_target)
: _texture_target(texture_target)
, _txo(INVALID_VAL)
, _is_loaded(false)
{}

bool gl_app::TextureBase::IsLoaded()
{
    return _is_loaded;
}

gl_app::TextureBase& gl_app::TextureBase::Bind(GLenum texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
    
    glActiveTexture(texture_unit);
    glBindTexture(_texture_target, _txo);

    return *this;
}

gl_app::TextureCubeMap::TextureCubeMap(const std::string & posx_file, const std::string & negx_file, const std::string & posy_file, const std::string & negy_file, const std::string & posz_file, const std::string & negz_file)
: TextureBase()
{
    _is_loaded = false;
    _texture_target = GL_TEXTURE_CUBE_MAP;
    _txo = INVALID_VAL;
    _filenames.insert(std::pair<std::string, GLenum>(posx_file, GL_TEXTURE_CUBE_MAP_POSITIVE_X));
    _filenames.insert(std::pair<std::string, GLenum>(negx_file, GL_TEXTURE_CUBE_MAP_NEGATIVE_X));
    _filenames.insert(std::pair<std::string, GLenum>(posy_file, GL_TEXTURE_CUBE_MAP_POSITIVE_Y));
    _filenames.insert(std::pair<std::string, GLenum>(negy_file, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y));
    _filenames.insert(std::pair<std::string, GLenum>(posz_file, GL_TEXTURE_CUBE_MAP_POSITIVE_Z));
    _filenames.insert(std::pair<std::string, GLenum>(negz_file, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z));
}

gl_app::TextureCubeMap::~TextureCubeMap()
{
    if (glfwGetCurrentContext() != nullptr && _txo != INVALID_VAL)
    {
        glDeleteTextures(1, &_txo);
        _txo = INVALID_VAL;
    }
}

gl_app::TextureCubeMap& gl_app::TextureCubeMap::Load()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glGenTextures(1, &_txo);
    glBindTexture(_texture_target, _txo);

    std::for_each(_filenames.begin(), _filenames.end(), [this](const std::pair<std::string, GLenum>& pair)
    {   
        GLint width, height, components;
        unsigned char* pixels = stbi_load(pair.first.c_str(), &width, &height, &components, STBI_rgb_alpha);
        if (pixels == nullptr) throw std::runtime_error("STBI could not load pixel information for image \"" + pair.first + "\"");

        glTexImage2D(pair.second, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(_texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        delete[] pixels;
        pixels = nullptr;
    });

    _is_loaded = true;

    return *this;
}

gl_app::Texture1DRandom::Texture1DRandom(size_t size)
: TextureBase::TextureBase()
{
    _texture_target = GL_TEXTURE_1D;
}

gl_app::Texture1DRandom & gl_app::Texture1DRandom::Load()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glm::vec3* data = new glm::vec3[_size];
    for (GLuint i = 0; i < _size; ++i)
    {
        data[i].x = GenRandFloat();
        data[i].y = GenRandFloat();
        data[i].z = GenRandFloat();
    }

    glGenTextures(1, &_txo);
    glBindTexture(_texture_target, _txo);
    glTexImage1D(_texture_target, 0, GL_RGB, ConvertToGLint(_size), 0, GL_RGB, GL_FLOAT, data);
    glTexParameterf(_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete[] data;

    GLuint err = glGetError();
    if (err != GL_NO_ERROR && err != GL_INVALID_ENUM) throw std::runtime_error("Error loading random texture. Error code: " + std::to_string(err));

    _is_loaded = true;

    return *this;
}

gl_app::Texture1DRandom & gl_app::Texture1DRandom::SetSize(size_t size)
{
    _size = size;

    _is_loaded = false;
    return *this;
}
