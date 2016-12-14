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
	/// OLD
    //private:
    //    breene::Camera& Init();
    //    breene::Camera& Update();
    //    breene::Camera& IncrementVelocity();
    //public:
    //    Camera();
    //    Camera(GLulong window_width, GLulong window_height);
    //    Camera(GLulong window_width, GLulong window_height, const glm::vec3& target, const glm::vec3& position, const glm::vec3& up);
    //    const GLulong GetHeight() const { return _wnd_height; }
    //    const GLulong GetWidth() const { return _wnd_width; }
    //    const GLfloat GetStepSize() const { return _step_size; }

    //    breene::Camera& SetStepSize(GLfloat step_size);
    //    breene::Camera& ResetStep();
    //    breene::Camera& Reset();
    //    breene::Camera& MoveForward();
    //    breene::Camera& MoveLeft();
    //    breene::Camera& MoveBackward();
    //    breene::Camera& MoveRight();

    //    // rotate the camera either vertically or horizontally, depending on the mouse position
    //    breene::Camera& MouseInput(GLdouble x, GLdouble y);

    //    breene::Camera& OnRender();
    //private:
    //    glm::vec3 _look_at;
    //    glm::vec3 _eye;
    //    glm::vec3 _up;

    //    GLfloat _horizontal_angle;
    //    GLfloat _vertical_angle;

    //    GLulong _wnd_height;
    //    GLulong _wnd_width;

    //    GLfloat _velocity;
    //    GLfloat _step_size;

    //    bool _top_edge;
    //    bool _bottom_edge;
    //    bool _left_edge;
    //    bool _right_edge;

    //    glm::dvec2 _mouse_pos;
    };
}
