#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>

namespace breene
{
    class Camera
    {
    private:
        breene::Camera& Init();
        breene::Camera& Update();
        breene::Camera& IncrementStep();
    public:
        Camera();
        Camera(GLulong window_width, GLulong window_height);
        Camera(GLulong window_width, GLulong window_height, const glm::vec3& target, const glm::vec3& position, const glm::vec3& up);
        // target
        const glm::vec3& GetLookAt() const { return _look_at; }
        // position
        const glm::vec3& GetEye() const { return _eye; }
        // up
        const glm::vec3& GetUp() const { return _up; }
        const GLulong GetHeight() const { return _wnd_height; }
        const GLulong GetWidth() const { return _wnd_width; }
        const GLfloat GetStepSize() const { return _step_size; }

        breene::Camera& SetStepSize(GLfloat step_size);
        breene::Camera& ResetStep();
        breene::Camera& Reset();
        //// handle keyboard input, and move the camera either forward/backward, or to the left/right, depending on the key being pressed
        //bool KeyInput(GLint key);
        breene::Camera& MoveForward();
        breene::Camera& MoveLeft();
        breene::Camera& MoveBackward();
        breene::Camera& MoveRight();

        // handle mouse input, and rotate the camera either vertically or horizontally, depending on the mouse position
        breene::Camera& MouseInput(GLdouble x, GLdouble y);

        breene::Camera& OnRender();
    private:
        glm::vec3 _look_at;
        glm::vec3 _eye;
        glm::vec3 _up;

        GLfloat _horizontal_angle;
        GLfloat _vertical_angle;

        GLulong _wnd_height;
        GLulong _wnd_width;

        GLfloat _step;
        GLfloat _step_size;

        bool _top_edge;
        bool _bottom_edge;
        bool _left_edge;
        bool _right_edge;

        glm::dvec2 _mouse_pos;
    };
}

