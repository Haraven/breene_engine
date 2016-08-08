#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <utility>
#include <string>
#include <vector>
#include "mesh.h"

// Windows-only: changes the value of the first argument to the window width, and that of the second argument to the window height
void GetDesktopResolution(unsigned long& width, unsigned long& height);
std::pair<unsigned long, unsigned long> GetDesktopResolution();

std::string GetFileAsString(const std::string& file_path);

inline glm::mat4 Multiply(const glm::mat4& left, const glm::mat4& right)
{
    glm::mat4 ret;

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            ret[i][j] = left[i][0] * right[0][j] +
                left[i][1] * right[1][j] +
                left[i][2] * right[2][j] +
                left[i][3] * right[3][j];
        }
    }

    return ret;
}

void CalculateNormals(const std::vector<GLuint>& indices, std::vector<gl_app::Vertex>& vertices);

inline GLuint ConvertToGLuint(size_t what)
{
    if (what > UINT_MAX) throw std::runtime_error("Size " + std::to_string(what) + " exceeds GLuint range");

    return static_cast<GLuint>(what);
}

inline GLint ConvertToGLint(size_t what)
{
    if (what > INT_MAX) throw std::runtime_error("Size " + std::to_string(what) + " exceeds GLint range");

    return static_cast<GLint>(what);
}

inline GLfloat GenRandFloat()
{
    return static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
}