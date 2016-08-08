#include "utils.h"
#include <wtypes.h>
#include <fstream>
#include <algorithm>

void GetDesktopResolution(unsigned long& width, unsigned long& height)
{
	RECT wnd;

	const HWND hdesktop = GetDesktopWindow();
	GetWindowRect(hdesktop, &wnd);

	width = wnd.right;
	height = wnd.bottom;
}

std::pair<unsigned long, unsigned long> GetDesktopResolution()
{
	unsigned long width, height;
	GetDesktopResolution(width, height);
	return std::pair<unsigned long, unsigned long>(width, height);
}

std::string GetFileAsString(const std::string& file_path)
{
	std::ifstream ifs(file_path);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return content;
}

void CalculateNormals(const std::vector<GLuint>& indices, std::vector<gl_app::Vertex>& vertices)
{
    if (indices.size() < 3)
        return;

    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
        unsigned int index_0 = indices[i],
            index_1          = indices[i + 1],
            index_2          = indices[i + 2];

        glm::vec3 v1 = vertices[index_1].pos - vertices[index_0].pos,
            v2 = vertices[index_2].pos - vertices[index_0].pos,
            normal = glm::normalize(glm::cross(v1, v2));

        vertices[index_0].normal += normal;
        vertices[index_1].normal += normal;
        vertices[index_2].normal += normal;
    }

    std::for_each(vertices.begin(), vertices.end(), [](gl_app::Vertex vert)
    {
        vert.normal = glm::normalize(vert.normal);
    });
}
