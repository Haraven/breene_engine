#include "camera.h"
#include <iostream>


void breene::Camera::Update()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_look_at = glm::normalize(front);

	_right = glm::normalize(glm::cross(_look_at, _world_up));
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
