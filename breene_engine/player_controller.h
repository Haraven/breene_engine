#pragma once

#include <GL\glew.h>
#include "camera.h"
#include "input_manager.h"

namespace breene
{
	class PlayerController
	{
	public:
		PlayerController();
		PlayerController(Camera* camera);

		Camera* GetCamera() const { return _camera; }
		breene::PlayerController& SetCamera(Camera* cam);
		breene::PlayerController& KeyPress(GLint glfw_key, GLint glfw_action);
		breene::PlayerController& MouseButtonPress(GLint glfw_key, GLint glfw_action);
		breene::PlayerController& HandleInput();
		breene::PlayerController& MouseMove(GLdouble x, GLdouble y);
		breene::PlayerController& MouseScroll(GLdouble x, GLdouble y);
		breene::PlayerController& SetTimeDelta(GLfloat delta);
		//breene::PlayerController& UpdateCamera();

		~PlayerController();
	private:
		GLfloat _time_delta;
		KeyboardManager _key_manager;
		MouseManager _mouse_manager;
		Camera* _camera;
	};
}