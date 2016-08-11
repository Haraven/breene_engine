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

void CalculateNormals(const std::vector<GLuint>& indices, std::vector<breene::Vertex>& vertices)
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

    std::for_each(vertices.begin(), vertices.end(), [](breene::Vertex vert)
    {
        vert.normal = glm::normalize(vert.normal);
    });
}


transform::Transformation::Transformation()
    : _scale(1.0f)
    , _rotation(0.0f)
    , _translation(0.0f)
    , _transformation_order(TRANSLATION_FIRST | ROTATION_SECOND | SCALE_THIRD)
    , _computed_transforms(NONE_SET)
{}

transform::Transformation& transform::Transformation::Scaling(GLfloat amount)
{
    return Scaling(amount, amount, amount);
}

transform::Transformation& transform::Transformation::Scaling(const glm::vec3& scale)
{
    return Scaling(scale.x, scale.y, scale.z);
}

transform::Transformation& transform::Transformation::Scaling(GLfloat x, GLfloat y, GLfloat z)
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;

    _computed_transforms &= ~SCALE_SET & ~WORLD_TRANSFORM_SET & ~WORLD_VIEW_PERSPECTIVE_SET & ~WORLD_PERSPECTIVE_TRANSFORM_SET; // turn off the scale transform and world bits

    return *this;
}

transform::Transformation& transform::Transformation::Translation(GLfloat x, GLfloat y, GLfloat z)
{
    _translation.x = x;
    _translation.y = y;
    _translation.z = z;

    _computed_transforms &= ~TRANSLATION_SET & ~VIEW_TRANSFORM_SET & ~WORLD_VIEW_PERSPECTIVE_SET & ~WORLD_TRANSFORM_SET & ~WORLD_PERSPECTIVE_TRANSFORM_SET; // turn off the translation and world transform bit

    return *this;
}

transform::Transformation& transform::Transformation::Translation(const glm::vec3& translation)
{
    return Translation(translation.x, translation.y, translation.z);
}

transform::Transformation& transform::Transformation::Rotation(GLfloat x, GLfloat y, GLfloat z)
{
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;

    _computed_transforms &= ~ROTATION_SET & ~VIEW_TRANSFORM_SET & ~WORLD_VIEW_PERSPECTIVE_SET & ~WORLD_TRANSFORM_SET & ~WORLD_PERSPECTIVE_TRANSFORM_SET; // turn off the rotation and world transform bit

    return *this;
}

transform::Transformation& transform::Transformation::Rotation(const glm::vec3& rotation)
{
    return Rotation(rotation.x, rotation.y, rotation.z);
}

transform::Transformation & transform::Transformation::PerspectiveProjection(GLfloat fov, GLulong width, GLulong height, GLfloat z_near, GLfloat z_far)
{
    _perspective_projection.fov = fov;
    _perspective_projection.width = width;
    _perspective_projection.height = height;
    _perspective_projection.z_near = z_near;
    _perspective_projection.z_far = z_far;

    _computed_transforms &= ~PERSPECTIVE_SET & ~WORLD_PERSPECTIVE_TRANSFORM_SET & WORLD_VIEW_PERSPECTIVE_SET;

    return *this;
}

transform::Transformation & transform::Transformation::PerspectiveProjection(const PerspectiveProjectionInfo & info)
{
    return PerspectiveProjection(info.fov, info.width, info.height, info.z_near, info.z_far);
}

transform::Transformation& transform::Transformation::Cam(const glm::vec3 & eye, const glm::vec3 & look_at, const glm::vec3 & up)
{
    _camera_info.eye = eye;
    _camera_info.look_at = look_at;
    _camera_info.up = up;

    _computed_transforms &= ~VIEW_TRANSFORM_SET & ~WORLD_VIEW_PERSPECTIVE_SET;

    return *this;
}

transform::Transformation& transform::Transformation::Cam(const breene::Camera& cam)
{
    if (&cam != nullptr)
        Cam(cam.GetEye(), cam.GetLookAt(), cam.GetUp());

    return *this;
}

transform::Transformation & transform::Transformation::Reset()
{
    _computed_transforms = NONE_SET;

    return *this;
}

glm::mat4 transform::Transformation::WorldTransform()
{
    if (!(_computed_transforms & WORLD_TRANSFORM_SET))
    {
        _scale_transform = _computed_transforms & SCALE_SET ? _scale_transform : ScalingTransform(_scale);
        _rotation_transform = _computed_transforms & ROTATION_SET ? _rotation_transform : RotationTransform(_rotation);
        _translation_transform = _computed_transforms & TRANSLATION_SET ? _translation_transform : TranslationTransform(_translation);

        if (_transformation_order & SCALE_FIRST)
            _world_transform = _scale_transform;
        else if (_transformation_order & ROTATION_FIRST)
            _world_transform = _rotation_transform;
        else
            _world_transform = _translation_transform;

        if (_transformation_order & SCALE_SECOND)
            _world_transform = Multiply(_world_transform, _scale_transform);
        else if (_transformation_order & ROTATION_SECOND)
            _world_transform = Multiply(_world_transform, _rotation_transform);
        else
            _world_transform = Multiply(_world_transform, _translation_transform);

        if (_transformation_order & SCALE_THIRD)
            _world_transform = Multiply(_world_transform, _scale_transform);
        else if (_transformation_order & ROTATION_THIRD)
            _world_transform = Multiply(_world_transform, _rotation_transform);
        else
            _world_transform = Multiply(_world_transform, _translation_transform);

        _computed_transforms |= WORLD_TRANSFORM_SET;
    }

    return _world_transform;
}

glm::mat4 transform::Transformation::PerspectiveWorldTransform()
{
    if (!(_computed_transforms & WORLD_PERSPECTIVE_TRANSFORM_SET))
    {
        _perspective_transform = _computed_transforms & PERSPECTIVE_SET ? _perspective_transform : PerspectiveTransform(_perspective_projection);
        _world_transform = _computed_transforms & WORLD_TRANSFORM_SET ? _world_transform : WorldTransform();

        _world_perspective_transform = Multiply(_perspective_transform, _world_transform);

        _computed_transforms |= WORLD_PERSPECTIVE_TRANSFORM_SET;
    }

    return _world_perspective_transform;
}

glm::mat4 transform::Transformation::WVPTransform()
{
    if (!(_computed_transforms & WORLD_VIEW_PERSPECTIVE_SET))
    {
        _world_transform = _computed_transforms & WORLD_TRANSFORM_SET ? _world_transform : WorldTransform();
        glm::mat4 view_perspective_transform = VPTransform();

        _world_view_perspective_transform = Multiply(view_perspective_transform, _world_transform);
        _computed_transforms |= WORLD_VIEW_PERSPECTIVE_SET;
    }

    return _world_view_perspective_transform;
}

transform::Transformation& transform::Transformation::SetTransformOrder(uint16_t order)
{
    _transformation_order = order;
    _computed_transforms &= WORLD_PERSPECTIVE_TRANSFORM_SET & ~WORLD_TRANSFORM_SET; // reset the world transform, since the order of the transformation has changed

    return *this;
}

glm::mat4 transform::Transformation::ScalingTransform(const glm::vec3 & scale)
{
    _scale_transform[0][0] = scale.x; _scale_transform[0][1] = 0.0f;    _scale_transform[0][2] = 0.0f;    _scale_transform[0][3] = 0.0f;
    _scale_transform[1][0] = 0.0f;    _scale_transform[1][1] = scale.y; _scale_transform[1][2] = 0.0f;    _scale_transform[1][3] = 0.0f;
    _scale_transform[2][0] = 0.0f;    _scale_transform[2][1] = 0.0f;    _scale_transform[2][2] = scale.z; _scale_transform[2][3] = 0.0f;
    _scale_transform[3][0] = 0.0f;    _scale_transform[3][1] = 0.0f;    _scale_transform[3][2] = 0.0f;    _scale_transform[3][3] = 1.0f;

    _computed_transforms |= SCALE_SET;

    return _scale_transform;
}

glm::mat4 transform::Transformation::RotationTransform(const glm::vec3 & rotation)
{
    glm::vec3 rotation_radians = glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
    glm::mat4 rx, ry, rz;

    rx[0][0] = 1.0f; rx[0][1] = 0.0f;                 rx[0][2] = 0.0f;                  rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = glm::cos(rotation_radians.x); rx[1][2] = -glm::sin(rotation_radians.x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = glm::sin(rotation_radians.x); rx[2][2] = glm::cos(rotation_radians.x);  rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f;                 rx[3][2] = 0.0f;                  rx[3][3] = 1.0f;

    ry[0][0] = glm::cos(rotation_radians.y); ry[0][1] = 0.0f; ry[0][2] = -glm::sin(rotation_radians.y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f;                 ry[1][1] = 1.0f; ry[1][2] = 0.0f;                  ry[1][3] = 0.0f;
    ry[2][0] = glm::sin(rotation_radians.y); ry[2][1] = 0.0f; ry[2][2] = glm::cos(rotation_radians.y);  ry[2][3] = 0.0f;
    ry[3][0] = 0.0f;                 ry[3][1] = 0.0f; ry[3][2] = 0.0f;                  ry[3][3] = 1.0f;

    rz[0][0] = glm::cos(rotation_radians.z); rz[0][1] = -glm::sin(rotation_radians.z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = glm::sin(rotation_radians.z); rz[1][1] = glm::cos(rotation_radians.z);  rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f;                 rz[2][1] = 0.0f;                  rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f;                 rz[3][1] = 0.0f;                  rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    _rotation_transform = rx * ry * rz;
    _computed_transforms |= ROTATION_SET;

    return _rotation_transform;
}

glm::mat4 transform::Transformation::TranslationTransform(const glm::vec3& translation)
{
    _translation_transform[0][0] = 1.0f; _translation_transform[0][1] = 0.0f; _translation_transform[0][2] = 0.0f; _translation_transform[0][3] = translation.x;
    _translation_transform[1][0] = 0.0f; _translation_transform[1][1] = 1.0f; _translation_transform[1][2] = 0.0f; _translation_transform[1][3] = translation.y;
    _translation_transform[2][0] = 0.0f; _translation_transform[2][1] = 0.0f; _translation_transform[2][2] = 1.0f; _translation_transform[2][3] = translation.z;
    _translation_transform[3][0] = 0.0f; _translation_transform[3][1] = 0.0f; _translation_transform[3][2] = 0.0f; _translation_transform[3][3] = 1.0f;

    _computed_transforms |= TRANSLATION_SET;

    return _translation_transform;
}

glm::mat4 transform::Transformation::PerspectiveTransform(const PerspectiveProjectionInfo & info)
{
    const GLfloat aspect_ratio = static_cast<GLfloat>(info.width) / static_cast<GLfloat>(info.height);
    const GLfloat z_near_minus_z_far = info.z_near - info.z_far;
    const GLfloat tan_half_fov = glm::tan(glm::radians(info.fov / 2.0f));
    _perspective_transform[0][0] = 1.0f / (tan_half_fov * aspect_ratio); _perspective_transform[0][1] = 0.0f;                _perspective_transform[0][2] = 0.0f;                                             _perspective_transform[0][3] = 0.0;
    _perspective_transform[1][0] = 0.0f;                                 _perspective_transform[1][1] = 1.0f / tan_half_fov; _perspective_transform[1][2] = 0.0f;                                             _perspective_transform[1][3] = 0.0;
    _perspective_transform[2][0] = 0.0f;                                 _perspective_transform[2][1] = 0.0f;                _perspective_transform[2][2] = -(info.z_near + info.z_far) / z_near_minus_z_far; _perspective_transform[2][3] = 2.0f * info.z_far * info.z_near / z_near_minus_z_far;
    _perspective_transform[3][0] = 0.0f;                                 _perspective_transform[3][1] = 0.0f;                _perspective_transform[3][2] = 1.0f;                                             _perspective_transform[3][3] = 0.0;

    _computed_transforms |= PERSPECTIVE_SET;

    return _perspective_transform;
}

glm::mat4 transform::Transformation::CameraTransform(const glm::vec3 & look_at, const glm::vec3 & up)
{
    glm::vec3 N = glm::normalize(look_at),
        U = glm::cross(glm::normalize(up), N),
        V = glm::cross(N, U);

    _camera_transform[0][0] = U.x; _camera_transform[0][1] = U.y; _camera_transform[0][2] = U.z; _camera_transform[0][3] = 0.0f;
    _camera_transform[1][0] = V.x; _camera_transform[1][1] = V.y; _camera_transform[1][2] = V.z; _camera_transform[1][3] = 0.0f;
    _camera_transform[2][0] = N.x; _camera_transform[2][1] = N.y; _camera_transform[2][2] = N.z; _camera_transform[2][3] = 0.0f;
    _camera_transform[3][0] = 0.0f; _camera_transform[3][1] = 0.0f; _camera_transform[3][2] = 0.0f; _camera_transform[3][3] = 1.0f;

    _computed_transforms |= CAMERA_TRANSFORM_SET;

    return _camera_transform;
}

glm::mat4 transform::Transformation::ViewTransform()
{
    glm::vec3 eye = _camera_info.eye;
    glm::mat4 camera_translation_transform = TranslationTransform(glm::vec3(-eye.x, -eye.y, -eye.z)),
        camera_rotation_transform = CameraTransform(_camera_info.look_at, _camera_info.up);

    _computed_transforms &= ~TRANSLATION_SET;

    _view_transform = Multiply(camera_rotation_transform, camera_translation_transform);
    _computed_transforms |= VIEW_TRANSFORM_SET;

    return _view_transform;
}

glm::mat4 transform::Transformation::VPTransform()
{
    _view_transform = _computed_transforms & VIEW_TRANSFORM_SET ? _view_transform : ViewTransform();
    _perspective_transform = _computed_transforms & PERSPECTIVE_SET ? _perspective_transform : PerspectiveTransform(_perspective_projection);

    return Multiply(_perspective_transform, _view_transform);
}
