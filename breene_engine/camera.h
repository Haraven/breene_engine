#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "my_constants.h"

namespace breene
{
    class Camera
    {
	private:

		void Update();
	public:
		enum CameraDirection
		{
			FORWARD  = 0x01,
			BACKWARD = 0x02,
			LEFT     = 0x04,
			RIGHT    = 0x08
		};

		Camera(const glm::vec3& pos = EYE_ORIGIN, const glm::vec3& up = UP_ORIGIN, GLfloat yaw = YAW, GLfloat pitch = PITCH, GLfloat roll = ROLL);
		
		breene::Camera& KeyboardMove(CameraDirection direction, GLfloat time_delta);
		breene::Camera& MouseMove(GLfloat x, GLfloat y, GLboolean constrain_pitch = true);
		breene::Camera& ResetMoveSpeed();
		breene::Camera& Reset();

        // target
        const glm::vec3& GetLookAt() const { return _look_at; }
        // position
        const glm::vec3& GetEye() const { return _eye; }
        // up
        const glm::vec3& GetUp() const { return _up; }
	private:
		glm::vec3 _eye;
		glm::vec3 _look_at;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _world_up;

		GLfloat _yaw;
		GLfloat _pitch;
		GLfloat _roll;
		
		GLfloat _movement_speed;
		GLfloat _mouse_sensitivity;
    };
}

