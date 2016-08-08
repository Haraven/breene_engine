#include "camera.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\rotate_vector.hpp>
#include <iostream>

const GLfloat CAMERA_MAX_STEP  = 1.0f;
const GLfloat CAMERA_MIN_STEP  = 0.0f;
const GLfloat CAMERA_STEP_SIZE = 0.04f;
const GLint CAMERA_MARGIN      = 10;
const GLfloat EDGE_STEP        = 0.7f;
const GLfloat HV_SCALING       = 20.0f;

const GLulong DEFAULT_WIDTH    = 800L;
const GLulong DEFAULT_HEIGHT   = 600L;

glm::vec3 EYE_ORIGIN(0.0f, 0.0f, 0.0f);
glm::vec3 LOOK_AT_ORIGIN(0.0f, 0.0f, 1.0f);
glm::vec3 UP_ORIGIN(0.0f, 1.0f, 0.0f);

gl_app::Camera& gl_app::Camera::Init()
{
    glm::vec3 horizontal_target(_look_at.x, 0.0f, _look_at.z);
    horizontal_target = glm::normalize(horizontal_target);

    if (horizontal_target.z >= 0.0f)
        _horizontal_angle = horizontal_target.x >= 0.0f ? 360.0f - glm::degrees(glm::asin(horizontal_target.z)) : 180.0f + glm::degrees(glm::asin(horizontal_target.z));
    else
        _horizontal_angle = horizontal_target.x >= 0.0f ? glm::degrees(glm::asin(-horizontal_target.z)) : 90.0f + glm::degrees(glm::asin(-horizontal_target.z));
    _vertical_angle = -glm::degrees(glm::asin(_look_at.y));

    return *this;
}

gl_app::Camera& gl_app::Camera::Update()
{
    const glm::vec3 vertical_axis(0.0f, 1.0f, 0.0f);

    glm::vec3 view(1.0f, 0.0f, 0.0f);
    view = glm::rotate(view, glm::radians(_horizontal_angle), vertical_axis);
    view = glm::normalize(view);

    glm::vec3 horizontal_axis = glm::normalize(glm::cross(vertical_axis, view));
    view = glm::rotate(view, glm::radians(_vertical_angle), horizontal_axis);

    _look_at = glm::normalize(view);

    _up = glm::normalize(glm::cross(_look_at, horizontal_axis));

    return *this;
}

gl_app::Camera& gl_app::Camera::IncrementStep()
{
    _step = _step < CAMERA_MAX_STEP ? _step + _step_size : CAMERA_MAX_STEP;

    return *this;
}

gl_app::Camera::Camera()
: Camera(DEFAULT_WIDTH, DEFAULT_HEIGHT)
{}

gl_app::Camera::Camera(GLulong window_width, GLulong window_height)
: _wnd_width(window_width)
, _wnd_height(window_height)
, _eye(EYE_ORIGIN)
, _look_at(LOOK_AT_ORIGIN)
, _up(UP_ORIGIN)
, _step_size(CAMERA_STEP_SIZE)
, _step(CAMERA_MIN_STEP)
, _top_edge(false)
, _bottom_edge(false)
, _right_edge(false)
, _left_edge(false)
, _mouse_pos(static_cast<GLfloat>(window_width) / 2.0f, static_cast<GLfloat>(window_height) / 2.0f)
{
    Init();
}

gl_app::Camera::Camera(GLulong window_width, GLulong window_height, const glm::vec3& target, const glm::vec3& position, const glm::vec3& up)
: _wnd_width(window_width)
, _wnd_height(window_height)
, _eye(position)
, _look_at(glm::normalize(target))
, _up(glm::normalize(up))
, _step_size(CAMERA_STEP_SIZE)
, _step(CAMERA_MIN_STEP)
{
    EYE_ORIGIN = _eye;
    LOOK_AT_ORIGIN = _look_at;
    UP_ORIGIN = _up;
    Init();
}

gl_app::Camera& gl_app::Camera::SetStepSize(GLfloat step_size)
{
    _step_size = glm::clamp(step_size, 0.0f, 1.0f);

    return *this;
}

gl_app::Camera& gl_app::Camera::ResetStep() 
{ 
    _step = CAMERA_MIN_STEP;

    return *this; 
}

gl_app::Camera & gl_app::Camera::Reset()
{
    _eye         = EYE_ORIGIN;
    _look_at     = LOOK_AT_ORIGIN;
    _up          = UP_ORIGIN;
    _step_size   = CAMERA_STEP_SIZE;
    _step        = CAMERA_MIN_STEP;
    _top_edge    = false;
    _bottom_edge = false;
    _right_edge  = false;
    _left_edge   = false;
    _mouse_pos   = glm::dvec2(static_cast<GLfloat>(_wnd_width) / 2.0f, static_cast<GLfloat>(_wnd_height) / 2.0f);

    Init();

    return *this;
}

gl_app::Camera& gl_app::Camera::MoveForward()
{
    IncrementStep();
    _eye += _look_at * _step;

    return *this;
}

gl_app::Camera& gl_app::Camera::MoveLeft()
{
    IncrementStep();
    glm::vec3 left = glm::normalize(glm::cross(_look_at, _up));
    left *= _step;
    _eye += left;

    return *this;
}

gl_app::Camera& gl_app::Camera::MoveBackward()
{
    IncrementStep();
    _eye -= _look_at * _step;

    return *this;
}

gl_app::Camera & gl_app::Camera::MoveRight()
{
    IncrementStep();
    glm::vec3 right = glm::normalize(glm::cross(_up, _look_at));
    right *= _step;
    _eye += right;

    return *this;
}

gl_app::Camera& gl_app::Camera::MouseInput(GLdouble x, GLdouble y)
{
    const GLint delta_x = static_cast<GLint>(glm::round(x - _mouse_pos.x));
    const GLint delta_y = static_cast<GLint>(glm::round(y - _mouse_pos.y));
    _mouse_pos.x = x;
    _mouse_pos.y = y;

    _horizontal_angle += static_cast<GLfloat>(delta_x) / HV_SCALING;
    _vertical_angle   += static_cast<GLfloat>(delta_y) / HV_SCALING;

    if (delta_x == 0)
    {
        if (x <= CAMERA_MARGIN)
            _left_edge = true;
        else if (x >= (_wnd_width - CAMERA_MARGIN))
            _right_edge = true;
    }
    else
        _left_edge = _right_edge = false;

    if (delta_y == 0)
    {
        if (y <= CAMERA_MARGIN)
            _top_edge = true;
        else if (y >= (_wnd_height - CAMERA_MARGIN))
            _bottom_edge = true;
    }
    else
        _top_edge = _bottom_edge = false;

    Update();

    return *this;
}

gl_app::Camera & gl_app::Camera::OnRender()
{
    bool should_update = false;

    if (_left_edge)
    {
        IncrementStep();
        _horizontal_angle -= _step;
        should_update = true;
    }
    else if (_right_edge)
    {
        IncrementStep();
        _horizontal_angle += _step;
        should_update = true;
    }
    else if (_top_edge)
    {
        if (_vertical_angle >= -90.0f)
        {
            IncrementStep();
            _vertical_angle -= _step;
            should_update = true;
        }
    }
    else if (_bottom_edge)
    {
        if (_vertical_angle < 90.0f)
        {
            IncrementStep();
            _vertical_angle += _step;
            should_update = true;
        }
    }

    if (should_update)
        Update();

    return *this;
}
