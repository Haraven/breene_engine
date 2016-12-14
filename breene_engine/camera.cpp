#include "camera.h"
#include <iostream>
//#include <glm\gtx\rotate_vector.hpp>

//const GLfloat CAMERA_MAX_STEP  = 1.0f;
//const GLfloat CAMERA_MIN_STEP  = 0.4f;
//const GLfloat CAMERA_STEP_SIZE = 0.04f;
//const GLint CAMERA_MARGIN      = 10;
//const GLfloat EDGE_STEP        = 0.7f;
//const GLfloat HV_SCALING       = 20.0f;

//const GLulong DEFAULT_WIDTH    = 800L;
//const GLulong DEFAULT_HEIGHT   = 600L;

/// OLD
//breene::Camera& breene::Camera::Init()
//{
//    glm::vec3 horizontal_target(_look_at.x, 0.0f, _look_at.z);
//    horizontal_target = glm::normalize(horizontal_target);
//
//    if (horizontal_target.z >= 0.0f)
//        _horizontal_angle = horizontal_target.x >= 0.0f ? 360.0f - glm::degrees(glm::asin(horizontal_target.z)) : 180.0f + glm::degrees(glm::asin(horizontal_target.z));
//    else
//        _horizontal_angle = horizontal_target.x >= 0.0f ? glm::degrees(glm::asin(-horizontal_target.z)) : 90.0f + glm::degrees(glm::asin(-horizontal_target.z));
//    _vertical_angle = -glm::degrees(glm::asin(_look_at.y));
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::Update()
//{
//    const glm::vec3 vertical_axis(0.0f, 1.0f, 0.0f);
//
//    glm::vec3 view(1.0f, 0.0f, 0.0f);
//    view = glm::rotate(view, glm::radians(_horizontal_angle), vertical_axis);
//    view = glm::normalize(view);
//
//    glm::vec3 horizontal_axis = glm::normalize(glm::cross(vertical_axis, view));
//    view = glm::rotate(view, glm::radians(_vertical_angle), horizontal_axis);
//
//    _look_at = glm::normalize(view);
//
//    _up = glm::normalize(glm::cross(_look_at, horizontal_axis));
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::IncrementVelocity()
//{
//    _velocity = _velocity < CAMERA_MAX_STEP ? _velocity + _step_size : CAMERA_MAX_STEP;
//
//    return *this;
//}
//
//breene::Camera::Camera()
//: Camera(DEFAULT_WIDTH, DEFAULT_HEIGHT)
//{}
//
//breene::Camera::Camera(GLulong window_width, GLulong window_height)
//: _wnd_width(window_width)
//, _wnd_height(window_height)
//, _eye(EYE_ORIGIN)
//, _look_at(LOOK_AT_ORIGIN)
//, _up(UP_ORIGIN)
//, _step_size(CAMERA_STEP_SIZE)
//, _velocity(CAMERA_MIN_STEP)
//, _top_edge(false)
//, _bottom_edge(false)
//, _right_edge(false)
//, _left_edge(false)
//, _mouse_pos(static_cast<GLfloat>(window_width) / 2.0f, static_cast<GLfloat>(window_height) / 2.0f)
//{
//    Init();
//}
//
//breene::Camera::Camera(GLulong window_width, GLulong window_height, const glm::vec3& target, const glm::vec3& position, const glm::vec3& up)
//: _wnd_width(window_width)
//, _wnd_height(window_height)
//, _eye(position)
//, _look_at(glm::normalize(target))
//, _up(glm::normalize(up))
//, _step_size(CAMERA_STEP_SIZE)
//, _velocity(CAMERA_MIN_STEP)
//{
//    EYE_ORIGIN = _eye;
//    LOOK_AT_ORIGIN = _look_at;
//    UP_ORIGIN = _up;
//    Init();
//}
//
//breene::Camera& breene::Camera::SetStepSize(GLfloat step_size)
//{
//    _step_size = glm::clamp(step_size, 0.0f, 1.0f);
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::ResetStep() 
//{ 
//    _velocity = CAMERA_MIN_STEP;
//
//    return *this; 
//}
//
//breene::Camera & breene::Camera::Reset()
//{
//    _eye         = EYE_ORIGIN;
//    _look_at     = LOOK_AT_ORIGIN;
//    _up          = UP_ORIGIN;
//    _step_size   = CAMERA_STEP_SIZE;
//    _velocity    = CAMERA_MIN_STEP;
//    _top_edge    = false;
//    _bottom_edge = false;
//    _right_edge  = false;
//    _left_edge   = false;
//    _mouse_pos   = glm::dvec2(static_cast<GLfloat>(_wnd_width) / 2.0f, static_cast<GLfloat>(_wnd_height) / 2.0f);
//
//    Init();
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::MoveForward()
//{
//    IncrementVelocity();
//    _eye += _look_at * _velocity;
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::MoveLeft()
//{
//    IncrementVelocity();
//    glm::vec3 left = glm::normalize(glm::cross(_look_at, _up));
//    left *= _velocity;
//    _eye += left;
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::MoveBackward()
//{
//    IncrementVelocity();
//    _eye -= _look_at * _velocity;
//
//    return *this;
//}
//
//breene::Camera & breene::Camera::MoveRight()
//{
//    IncrementVelocity();
//    glm::vec3 right = glm::normalize(glm::cross(_up, _look_at));
//    right *= _velocity;
//    _eye += right;
//
//    return *this;
//}
//
//breene::Camera& breene::Camera::MouseInput(GLdouble x, GLdouble y)
//{
//    const GLint delta_x = static_cast<GLint>(glm::round(x - _mouse_pos.x));
//    const GLint delta_y = static_cast<GLint>(glm::round(y - _mouse_pos.y));
//    _mouse_pos.x = x;
//    _mouse_pos.y = y;
//
//    _horizontal_angle += static_cast<GLfloat>(delta_x) / HV_SCALING;
//    _vertical_angle   += static_cast<GLfloat>(delta_y) / HV_SCALING;
//
//    if (delta_x == 0)
//    {
//        if (x <= CAMERA_MARGIN)
//            _left_edge = true;
//        else if (x >= (_wnd_width - CAMERA_MARGIN))
//            _right_edge = true;
//    }
//    else
//        _left_edge = _right_edge = false;
//
//    if (delta_y == 0)
//    {
//        if (y <= CAMERA_MARGIN)
//            _top_edge = true;
//        else if (y >= (_wnd_height - CAMERA_MARGIN))
//            _bottom_edge = true;
//    }
//    else
//        _top_edge = _bottom_edge = false;
//
//    Update();
//
//    return *this;
//}
//
//breene::Camera & breene::Camera::OnRender()
//{
//    bool should_update = false;
//
//    if (_left_edge)
//    {
//        IncrementVelocity();
//        _horizontal_angle -= _velocity;
//        should_update = true;
//    }
//    else if (_right_edge)
//    {
//        IncrementVelocity();
//        _horizontal_angle += _velocity;
//        should_update = true;
//    }
//    else if (_top_edge)
//    {
//        if (_vertical_angle >= -90.0f)
//        {
//            IncrementVelocity();
//            _vertical_angle -= _velocity;
//            should_update = true;
//        }
//    }
//    else if (_bottom_edge)
//    {
//        if (_vertical_angle < 90.0f)
//        {
//            IncrementVelocity();
//            _vertical_angle += _velocity;
//            should_update = true;
//        }
//    }
//
//    if (should_update)
//        Update();
//
//    return *this;
//}

void breene::Camera::Update()
{
	//_look_at = glm::normalize(glm::vec3
	//(
	//	glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch)),
	//	glm::sin(glm::radians(_pitch)),
	//	glm::sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
	//));

	//_right = glm::normalize(glm::cross(_look_at, _world_up));
	//_up    = glm::normalize(glm::cross(_right, _look_at));

	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_look_at = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	_right = glm::normalize(glm::cross(_look_at, _world_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_up = glm::normalize(glm::cross(_right, _look_at));
}

breene::Camera::Camera(const glm::vec3 & pos, const glm::vec3 & up, GLfloat yaw, GLfloat pitch, GLfloat roll)
: _eye(pos)
, _look_at(LOOK_AT_ORIGIN)
, _world_up(up)
, _pitch(pitch)
, _yaw(yaw)
, _movement_speed(MOVEMENT_SPEED_INCR)
, _mouse_sensitivity(MOUSE_SENSITIVTY)
{
	Update();
}

breene::Camera & breene::Camera::KeyboardMove(CameraDirection direction, GLfloat time_delta)
{
	GLfloat velocity = _movement_speed * time_delta;
	_movement_speed += MOVEMENT_SPEED_INCR / 3.0f;
	_movement_speed = glm::clamp(_movement_speed, 0.0f, MOVEMENT_SPEED_MAX);
	if (direction & FORWARD)
		_eye += _look_at * velocity;
	if (direction & BACKWARD)
		_eye -= _look_at * velocity;
	if (direction & RIGHT)
		_eye -= _right * velocity;
	if (direction & LEFT)
		_eye += _right * velocity;

	return *this;
}

breene::Camera & breene::Camera::MouseMove(GLfloat x, GLfloat y, GLboolean constrain_pitch)
{
	x *= _mouse_sensitivity;
	y *= _mouse_sensitivity;

	_yaw += x;
	_pitch += y;
	if (constrain_pitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}

	Update();

	return *this;
}

breene::Camera & breene::Camera::ResetMoveSpeed()
{
	_movement_speed = MOVEMENT_SPEED_INCR;

	return *this;
}

breene::Camera & breene::Camera::Reset()
{
	_eye      = EYE_ORIGIN;
	_world_up = UP_ORIGIN;
	_yaw      = YAW;
	_pitch    = PITCH;
	_roll     = ROLL;

	Update();

	return *this;
}
