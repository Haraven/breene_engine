#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <assimp\scene.h>
#include <vector>
#include "texture.h"
#include "callbacks.h"

namespace gl_app
{
    struct Vertex
    {
        Vertex() {}
        Vertex(const glm::vec3& position, const glm::vec2& texture_coord)
        : pos(position)
        , tex_coord(texture_coord)
        , normal(0.0f)
        , tangent(0.0f)
        {}
        Vertex(const glm::vec3& position, const glm::vec2& texture_coord, const glm::vec3& normal, const glm::vec3& tangent)
        : pos(position)
        , tex_coord(texture_coord)
        , normal(normal)
        , tangent(tangent)
        {}

        Vertex(const glm::vec3& position, const glm::vec2& texture_coord, const glm::vec3& normal)
        : pos(position)
        , tex_coord(texture_coord)
        , normal(normal)
        , tangent(0.0f)
        {}

        glm::vec3 pos;
        glm::vec2 tex_coord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };


    class Mesh
    {
    private:
        gl_app::Mesh& InitFromScene(const aiScene* scene, const std::string& filename);
        gl_app::Mesh& InitMesh(const aiMesh* mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords, std::vector<GLuint>& indices, std::vector<glm::vec3>& tangents = std::vector<glm::vec3>());
        gl_app::Mesh& InitMats(const aiScene* scene, const std::string& filename);
        gl_app::Mesh& Clear();

        const static size_t BUFFER_COUNT = 7;
        enum BufferIndex
        {
            INDEX_BUFFER           = 0,
            POS_VERTEX_BUFFER      = 1,
            NORMAL_VERTEX_BUFFER   = 2,
            TEXCOORD_VERTEX_BUFFER = 3,
            TANGENT_VERTEX_BUFFER  = 4,
            WVP_MAT_VERTEX_BUFFER  = 5,
            W_MAT_VERTEX_BUFFER    = 6
        };
    public:
        Mesh();

        gl_app::Mesh& Load(const std::string& filename);
        gl_app::Mesh& Render(RenderCallback* render_callback = nullptr, bool draw_patches = false);
        //gl_app::Mesh& Render(GLuint draw_index, GLuint primitive_id);
        gl_app::Mesh& Render(GLuint instance_count, const glm::mat4* wvp_mats, const glm::mat4* w_mats, RenderCallback* render_callback = nullptr, bool draw_patches = false);

        ~Mesh();
    private:
        class MeshEntry
        {
        public:
            MeshEntry();

            GLuint GetBaseVertex() const { return _base_vertex; }
            GLuint GetBaseIndex() const { return _base_index; }
            GLuint GetIndexCount() const { return _index_count; }
            GLuint GetMaterialIndex() const { return _material_index; }
            gl_app::Mesh::MeshEntry& SetMaterialIndex(GLuint index);
            gl_app::Mesh::MeshEntry& SetBaseVertex(GLuint vert);
            gl_app::Mesh::MeshEntry& SetIndexCount(GLuint count);
            gl_app::Mesh::MeshEntry& SetBaseIndex(GLuint index);
        private:
            GLuint _base_vertex;
            GLuint _base_index;
            GLuint _index_count;
            GLuint _material_index;
        };
        
        GLuint _vao;
        GLuint _buffers[BUFFER_COUNT];
        std::vector<MeshEntry> _meshes;
        std::vector<Texture2D*> _textures;
    };
}

