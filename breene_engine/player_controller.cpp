#include "player_controller.h"
#include <iostream>

breene::PlayerController::PlayerController()
: _camera(new Camera())
, _time_delta(0.0f)
{}

breene::PlayerController::PlayerController(Camera * camera)
: _camera(camera)
,_time_delta(0.0f)
{}

breene::PlayerController & breene::PlayerController::SetCamera(Camera * cam)
{
	if (_camera != nullptr)
		delete _camera;
	_camera = cam;

	return *this;
}

breene::PlayerController & breene::PlayerController::KeyPress(GLint glfw_key, GLint glfw_action)
{
	_key_manager.Input(glfw_key, glfw_action);

	return *this;
}

breene::PlayerController & breene::PlayerController::MouseButtonPress(GLint glfw_key, GLint glfw_action)
{
	_mouse_manager.Input(glfw_key, glfw_action);

	return *this;
}

breene::PlayerController & breene::PlayerController::HandleInput()
{
	GLint movement_keys_pressed = 0;
	if (_key_manager.GetKeyStatus(BREENE_KEY_W) == BREENE_KEY_PRESSED || _key_manager.GetKeyStatus(BREENE_KEY_UP) == BREENE_KEY_PRESSED)
	{
		++movement_keys_pressed;
		_camera->KeyboardMove(Camera::CameraDirection::FORWARD, _time_delta);
	}
	if (_key_manager.GetKeyStatus(BREENE_KEY_S) == BREENE_KEY_PRESSED || _key_manager.GetKeyStatus(BREENE_KEY_DOWN) == BREENE_KEY_PRESSED)
	{
		++movement_keys_pressed;
		_camera->KeyboardMove(Camera::CameraDirection::BACKWARD, _time_delta);
	}
	if (_key_manager.GetKeyStatus(BREENE_KEY_A) == BREENE_KEY_PRESSED || _key_manager.GetKeyStatus(BREENE_KEY_LEFT) == BREENE_KEY_PRESSED)
	{
		++movement_keys_pressed;
		_camera->KeyboardMove(Camera::CameraDirection::LEFT, _time_delta);
	}
	if (_key_manager.GetKeyStatus(BREENE_KEY_D) == BREENE_KEY_PRESSED || _key_manager.GetKeyStatus(BREENE_KEY_RIGHT) == BREENE_KEY_PRESSED)
	{
		++movement_keys_pressed;
		_camera->KeyboardMove(Camera::CameraDirection::RIGHT, _time_delta);
	}
	if (_key_manager.GetKeyStatus(BREENE_KEY_R) == BREENE_KEY_PRESSED)
		_camera->Reset();

	return *this;
}

breene::PlayerController & breene::PlayerController::MouseMove(GLdouble x, GLdouble y)
{
	_camera->MouseMove(static_cast<GLfloat>(x), static_cast<GLfloat>(y));

	return *this;
}

breene::PlayerController & breene::PlayerController::MouseScroll(GLdouble x, GLdouble y)
{
	if (y < 0.0f)
		_camera->KeyboardMove(Camera::CameraDirection::BACKWARD, _time_delta);
	else
		_camera->KeyboardMove(Camera::CameraDirection::FORWARD, _time_delta);

	return *this;
}

breene::PlayerController & breene::PlayerController::SetTimeDelta(GLfloat delta)
{
	_time_delta = delta;

	return *this;
}

breene::PlayerController::~PlayerController()
{
	if (_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}
}
