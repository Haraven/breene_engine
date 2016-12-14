#include <iostream>
#include "app.h"

using namespace breene;

BreeneApplication* app;

void ErrCallback(const int, const char*);
void KeyCallback(GLFWwindow*, GLint, GLint, GLint, GLint);
void MouseScrollCallback(GLFWwindow*, GLdouble, GLdouble);
void MouseMoveCallback(GLFWwindow*, GLdouble, GLdouble);
void MouseButtonCallback(GLFWwindow*, GLint, GLint, GLint);

int main()
{
    unsigned long desktop_width;
    unsigned long desktop_height;
    GetDesktopResolution(desktop_width, desktop_height);

    try
    {
		auto builder = BreeneApplication::GetBuilder();
		app = builder.Title("Breene")
			.Width(desktop_width)
			.Height(desktop_height)
			.BGColor(0.1f, 0.1f, 0.1f, 1.0f)
			.DrawDistance(static_cast<GLfloat>(desktop_width))
			.GLVersion(4, 2)
			.StatsToDisplay(DISPLAY_FPS | DISPLAY_UPTIME)
			.Vsync(true)
			.Fullscreen(true)
			.CaptureInput(true)
			.HideCursor(true)
			.DepthTest(true)
			.Build();

        app->SetErrorCallback(ErrCallback)
            .SetKeyCallback(KeyCallback)
            .SetCursorCallback(MouseMoveCallback)
            .SetMouseButtonCallback(MouseButtonCallback)
            .SetMouseScrollCallback(MouseScrollCallback)
            .Run();
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        delete app;
        return FAILURE;
    }

    delete app;
    return SUCCESS;
}

void ErrCallback(const int errcode, const char* msg)
{
    std::cerr << "Error " << errcode << ": " << msg << std::endl;
}

void KeyCallback(GLFWwindow * wnd, GLint key, GLint scan_code, GLint action, GLint modifiers)
{
    app->GetPlayerController()->KeyPress(key, action);

    switch (key)
    {
    case GLFW_KEY_EQUAL:
        app->SetTesselationAlpha(app->GetTesselationAlpha() + 0.1f);
        break;
    case GLFW_KEY_MINUS:
        app->SetTesselationAlpha(app->GetTesselationAlpha() - 0.1f);
        break;
    case GLFW_KEY_RIGHT_BRACKET:
    case GLFW_KEY_KP_ADD:
        app->SetTesselationLevel(app->GetTesselationLevel() + 0.1f);
        break;
    case GLFW_KEY_LEFT_BRACKET:
    case GLFW_KEY_KP_SUBTRACT:
        app->SetTesselationLevel(app->GetTesselationLevel() - 0.1f);
        break;
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(wnd, GL_TRUE);
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
            app->ToggleSpotLight(true);
        else if (action == GLFW_RELEASE)
            app->ToggleSpotLight(false);
        break;
    case GLFW_KEY_B:
        if (action == GLFW_PRESS)
            app->ToggleWireframe();
        break;
    default:
        break;
    }
}

void MouseScrollCallback(GLFWwindow* wnd, GLdouble x, GLdouble y)
{
    app->GetPlayerController()->MouseScroll(x, y);
}

void MouseMoveCallback(GLFWwindow* wnd, GLdouble x, GLdouble y)
{
    static GLdouble last_x = x;
    static GLdouble last_y = y;

    GLfloat x_offset = static_cast<GLfloat>(last_x - x);
    GLfloat y_offset = static_cast<GLfloat>(last_y - y);

    last_x = x;
    last_y = y;

    app->GetPlayerController()->MouseMove(x_offset, y_offset);
}

void MouseButtonCallback(GLFWwindow* wnd, GLint button, GLint action, GLint modifiers)
{
    app->GetPlayerController()->MouseButtonPress(button, action);
}