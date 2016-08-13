#pragma once

#include "mesh.h"
#include "camera.h"

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

void CalculateNormals(const std::vector<GLuint>& indices, std::vector<breene::Vertex>& vertices);

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

namespace transform
{
    enum
    {
        TRANSLATION_FIRST  = 0x01,
        ROTATION_FIRST     = 0x02,
        SCALE_FIRST        = 0x04,
        TRANSLATION_SECOND = 0x08,
        ROTATION_SECOND    = 0x10,
        SCALE_SECOND       = 0x20,
        TRANSLATION_THIRD  = 0x40,
        ROTATION_THIRD     = 0x80,
        SCALE_THIRD        = 0x100
    };

    struct PerspectiveProjectionInfo
    {
        GLfloat fov;
        GLulong width;
        GLulong height;
        GLfloat z_near;
        GLfloat z_far;
    };

    class Transformation
    {
    public:
        Transformation();
        Transformation& Scaling(GLfloat amount);
        Transformation& Scaling(const glm::vec3& scale);
        Transformation& Scaling(GLfloat x, GLfloat y, GLfloat z);
        Transformation& Translation(GLfloat x, GLfloat y, GLfloat z);
        Transformation& Translation(const glm::vec3& translation);
        Transformation& Rotation(GLfloat x, GLfloat y, GLfloat z);
        Transformation& Rotation(const glm::vec3& rotation);
        Transformation& PerspectiveProjection(GLfloat fov, GLulong width, GLulong height, GLfloat z_near, GLfloat z_far);
        Transformation& PerspectiveProjection(const PerspectiveProjectionInfo& info);
        Transformation& Cam(const glm::vec3& eye, const glm::vec3& look_at, const glm::vec3& up);
        Transformation& Cam(const breene::Camera& cam);
        Transformation& Reset();

        ///<summary>
        /// Retrieves the world transformation matrix obtained from the product of the rotation, translation, and scaling matrices.
        ///</summary>
        ///<remarks>
        /// The order of the multiplications is going to be translation - rotation - scaling, unless specified otherwise (via SetTransformOrder)
        ///</remarks>
        glm::mat4 WorldTransform();

        ///<summary>
        /// Retrieves the world transformation matrix, along with the perspective projection, obtained from the product of the perspective, rotation, translation, and scaling matrices.
        ///</summary>
        ///<remarks>
        /// The order of the multiplications is going to be translation - rotation - scaling, unless specified otherwise (via SetTransformOrder)
        ///</remarks>
        glm::mat4 PerspectiveWorldTransform();

        ///<summary>
        /// Retrieves the view perspective world transformation matrix, along with the perspective projection, obtained from the product of the perspective, rotation, translation, and scaling matrices.
        ///</summary>
        ///<remarks>
        /// The order of the multiplications is going to be translation - rotation - scaling, unless specified otherwise (via SetTransformOrder)
        ///</remarks>
        glm::mat4 WVPTransform();

        // retrieves the scale transformation matrix
        glm::mat4 ScalingTransform(const glm::vec3& scale);
        // retrieves the rotation transformation matrix, on all 3 axes. the rotation vector's components must be radians
        glm::mat4 RotationTransform(const glm::vec3& rotation);
        // retrieves the translation transformation matrix
        glm::mat4 TranslationTransform(const glm::vec3& translation);
        // retrieves the perspective projection matrix
        glm::mat4 PerspectiveTransform(const PerspectiveProjectionInfo& info);
        glm::mat4 CameraTransform(const glm::vec3& look_at, const glm::vec3& up);
        // retrieves the view transformation matrix for the camera
        glm::mat4 ViewTransform();
        glm::mat4 VPTransform();

        // sets the order in which the transformations should be applied, when computing transformation matrices. Use of bitmask constants defined in namespace transform is recommended. 
        Transformation& SetTransformOrder(uint16_t order);
    private:
        static const uint16_t
            NONE_SET = 0x00,
            TRANSLATION_SET = 0x01,
            ROTATION_SET = 0x02,
            SCALE_SET = 0x04,
            WORLD_TRANSFORM_SET = 0x08,
            PERSPECTIVE_SET = 0x10,
            WORLD_PERSPECTIVE_TRANSFORM_SET = 0x20,
            CAMERA_TRANSFORM_SET = 0x40,
            VIEW_TRANSFORM_SET = 0x80,
            WORLD_VIEW_PERSPECTIVE_SET = 0x100;

        struct CameraInfo
        {
            glm::vec3 eye;
            glm::vec3 look_at;
            glm::vec3 up;
        };

        uint16_t _transformation_order;
        uint16_t _computed_transforms;

        glm::vec3 _scale;
        glm::vec3 _translation;
        glm::vec3 _rotation;
        PerspectiveProjectionInfo _perspective_projection;
        CameraInfo _camera_info;

        glm::mat4 _scale_transform;
        glm::mat4 _translation_transform;
        glm::mat4 _rotation_transform;
        glm::mat4 _perspective_transform;
        glm::mat4 _world_transform;
        glm::mat4 _world_perspective_transform;
        glm::mat4 _camera_transform;
        glm::mat4 _view_transform;
        glm::mat4 _world_view_perspective_transform;
    };
}