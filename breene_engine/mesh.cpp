#include "mesh.h"
#include <GLFW\glfw3.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <iostream>
#include "my_constants.h"

breene::Mesh & breene::Mesh::InitFromScene(const aiScene * scene, const std::string & filename)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");
    if (scene == nullptr) throw std::runtime_error("ASSIMP scene was null");
    _meshes.resize(scene->mNumMeshes);
    _textures.resize(scene->mNumMaterials);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> tangents;
    std::vector<GLuint> indices;

    GLuint vertex_count = 0, index_count = 0;

    for (GLuint i = 0; i < _meshes.size(); ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        _meshes[i].SetMaterialIndex(mesh->mMaterialIndex)
            .SetIndexCount(mesh->mNumFaces * 3)
            .SetBaseVertex(vertex_count)
            .SetBaseIndex(index_count);
        vertex_count += mesh->mNumVertices;
        index_count += _meshes[i].GetIndexCount();
    }
    positions.reserve(vertex_count);
    normals.reserve(vertex_count);
    texcoords.reserve(vertex_count);
    tangents.reserve(vertex_count);

    for (int i = 0; i < _meshes.size(); ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        InitMesh(mesh, positions, normals, texcoords, indices, tangents);
    }

    InitMats(scene, filename);

    const GLuint POS_INDEX = 0,
        TEXCOORD_INDEX = 1, 
        NORMAL_INDEX = 2, 
        TANGENT_INDEX = 3,
        WVP_INDEX = 4,
        W_INDEX = 8;

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[POS_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(POS_INDEX);
    glVertexAttribPointer(POS_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[TEXCOORD_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEXCOORD_INDEX);
    glVertexAttribPointer(TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[NORMAL_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_INDEX);
    glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[TANGENT_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tangents.size(), tangents.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(TANGENT_INDEX);
    glVertexAttribPointer(TANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);


    const size_t MAT_ROWCOUNT = 4;
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[WVP_MAT_VERTEX_BUFFER]);
    for (GLuint i = 0; i < MAT_ROWCOUNT; ++i)
    {
        glEnableVertexAttribArray(WVP_INDEX + i);
        const GLvoid* offset = (const GLvoid*)(sizeof(GLfloat) * i * 4); // 4 floats every ith row
        glVertexAttribPointer(WVP_INDEX + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        glVertexAttribDivisor(WVP_INDEX + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[W_MAT_VERTEX_BUFFER]);
    for (GLuint i = 0; i < MAT_ROWCOUNT; ++i)
    {
        glEnableVertexAttribArray(W_INDEX + i);
        const GLvoid* offset = (const GLvoid*)(sizeof(GLfloat) * i * 4);
        glVertexAttribPointer(W_INDEX + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        glVertexAttribDivisor(W_INDEX + i, 1);
    }

    return *this;
}

breene::Mesh & breene::Mesh::InitMesh(const aiMesh* mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords, std::vector<GLuint>& indices, std::vector<glm::vec3>& tangents)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");

    const aiVector3D zero(0.0f);
    for (GLuint i = 0; i < mesh->mNumVertices; ++i)
    {
        const aiVector3D* pos = &(mesh->mVertices[i]);
        const aiVector3D* normal = &(mesh->mNormals[i]);
        const aiVector3D* tex_coord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;
        const aiVector3D* tangent = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[i]) : &zero;

        positions.push_back(glm::vec3(pos->x, pos->y, pos->z));
        normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
        texcoords.push_back(glm::vec2(tex_coord->x, tex_coord->y));
        tangents.push_back(glm::vec3(tangent->x, tangent->y, tangent->z));
    }

    for (GLuint i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    return *this;
}

breene::Mesh & breene::Mesh::InitMats(const aiScene * scene, const std::string & filename)
{
    std::string::size_type slash_index = filename.find_last_of("/");
    std::string directory;

    if (slash_index == std::string::npos) {
        directory = ".";
    }
    else if (slash_index == 0) {
        directory = "/";
    }
    else {
        directory = filename.substr(0, slash_index);
    }

    // Initialize the materials
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        const aiMaterial* material = scene->mMaterials[i];

        _textures[i] = nullptr;
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string path_data(path.data);
                if (path_data.substr(0, 2) == ".\\") {
                    path_data = path_data.substr(2, path_data.size() - 2);
                }

                std::string full_path = directory + "/" + path_data;
                try
                {
                    _textures[i] = new Texture2D(full_path.c_str(), GL_TEXTURE_2D);
                    _textures[i]->Load();
                }
                catch (std::runtime_error& e)
                {
                    delete _textures[i];
                    _textures[i] = nullptr;

                    std::cerr << "Error loading texture for mesh \"" << full_path << "\". Error reads: " << e.what() << std::endl;
                }
            }
        }
    }

    return *this;
}

breene::Mesh & breene::Mesh::Clear()
{
    std::for_each(_textures.begin(), _textures.end(), [](Texture2D* texture)
    {
        if (texture != nullptr)
        {
            delete texture;
            texture = nullptr;
        }
    });
    _textures.clear();

    if (sizeof(_buffers) > 0 && _buffers[0] != NULL)
    {
        glDeleteBuffers(BUFFER_COUNT, _buffers);
        for (GLuint i = 0; i < BUFFER_COUNT; ++i)
            _buffers[i] = NULL;
    }

    if (_vao != INVALID_VAL)
    {
        glDeleteBuffers(1, &_vao);
        _vao = INVALID_VAL;
    }

    return *this;
}

breene::Mesh::Mesh()
: _vao(INVALID_VAL)
{
    memset(_buffers, NULL, BUFFER_COUNT * sizeof(GLuint));
}

breene::Mesh& breene::Mesh::Load(const std::string & filename)
{
    if (!_textures.empty())
        Clear();
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
	glGenBuffers(BUFFER_COUNT, _buffers);

    Assimp::Importer importer;

    unsigned int importer_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_FindDegenerates;
    const aiScene* scene = importer.ReadFile(filename.c_str(), importer_flags);
    if (scene != nullptr)
        InitFromScene(scene, filename);
    else
        std::cerr << "Error reading from file \"" << filename << "\"" << ". Error reads: " << importer.GetErrorString();
    glBindVertexArray(0);

    return *this;
}

breene::Mesh & breene::Mesh::Render(RenderCallback* render_callback, bool draw_patches)
{
    GLuint index = 0;
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");

    glBindVertexArray(_vao);

    std::for_each(_meshes.begin(), _meshes.end(), [&index, &draw_patches, &render_callback, this](MeshEntry& entry)
    {
        const GLuint mat_index = entry.GetMaterialIndex();
        if (mat_index >= _textures.size()) throw std::out_of_range("Material index was out of range");
		if (_textures[mat_index] != nullptr)
		{
			if (!_textures[mat_index]->IsLoaded())
				_textures[mat_index]->Load();
			_textures[mat_index]->Bind(COLOR_TEXTURE_UNIT);
		}

        if (render_callback != nullptr)
        {
            render_callback->DrawStart(index);
            ++index;
        }

        const GLvoid* offset = (GLvoid*)(sizeof(GLuint) * entry.GetBaseIndex());
        glDrawElementsBaseVertex(draw_patches ? GL_PATCHES : GL_TRIANGLES, entry.GetIndexCount(), GL_UNSIGNED_INT, offset, entry.GetBaseVertex());
    });

    glBindVertexArray(0);

    return *this;
}

breene::Mesh & breene::Mesh::Render(GLuint instance_count, const glm::mat4* wvp_mats, const glm::mat4* w_mats, RenderCallback* render_callback, bool draw_patches)
{
    GLuint index = 0;

    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");

    const size_t element_count = sizeof(glm::mat4) * instance_count;
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[WVP_MAT_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, element_count, wvp_mats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _buffers[W_MAT_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, element_count, w_mats, GL_DYNAMIC_DRAW);

    glBindVertexArray(_vao);

    std::for_each(_meshes.begin(), _meshes.end(), [&instance_count, &render_callback, &draw_patches, &index, this](MeshEntry& entry)
    {
        const GLuint mat_index = entry.GetMaterialIndex();

        if (mat_index >= _textures.size()) throw std::out_of_range("Material index was out of range");

        if (_textures[mat_index] != nullptr)
        {
            if (!_textures[mat_index]->IsLoaded())
                _textures[mat_index]->Load();
            _textures[mat_index]->Bind(COLOR_TEXTURE_UNIT);

            if (render_callback != nullptr)
            {
                render_callback->DrawStart(index);
                ++index;
            }

            const GLvoid* offset = (GLvoid*)(sizeof(GLuint) * entry.GetBaseIndex());
            glDrawElementsInstancedBaseVertex(draw_patches ? GL_PATCHES : GL_TRIANGLES, entry.GetIndexCount(), GL_UNSIGNED_INT, offset, instance_count, entry.GetBaseVertex());
        }
    });

    glBindVertexArray(0);

    return *this;
}

//gl_app::Mesh & gl_app::Mesh::Render(GLuint draw_index, GLuint primitive_id)
//{
//    if (draw_index >= _meshes.size()) throw std::runtime_error("Draw index was out of range.");
//    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");
//
//
//
//    glBindVertexArray(_vao);
//    
//    const GLvoid* offset = (GLvoid*)(sizeof(GLuint) * _meshes[draw_index].GetBaseIndex());
//
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)(primitive_id * 3 * sizeof(GLuint)));
//
//    glBindVertexArray(0);
//    return *this;
//}

breene::Mesh::~Mesh()
{
    Clear();
}

breene::Mesh::MeshEntry::MeshEntry()
: _index_count(0)
, _base_vertex(0)
, _base_index(0)
, _material_index(INVALID_VAL)
{}

breene::Mesh::MeshEntry & breene::Mesh::MeshEntry::SetMaterialIndex(GLuint index)
{
    _material_index = index;

    return *this;
}

breene::Mesh::MeshEntry & breene::Mesh::MeshEntry::SetBaseVertex(GLuint vert)
{
    _base_vertex = vert;

    return *this;
}

breene::Mesh::MeshEntry & breene::Mesh::MeshEntry::SetIndexCount(GLuint count)
{
    _index_count = count;

    return *this;
}

breene::Mesh::MeshEntry & breene::Mesh::MeshEntry::SetBaseIndex(GLuint index)
{
    _base_index = index;

    return *this;
}
