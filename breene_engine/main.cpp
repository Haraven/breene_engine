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
		ApplicationBuilder builder = BreeneApplication::GetBuilder();
		app = builder.Title("Breene")
			.Width(desktop_width)
			.Height(desktop_height)
			.BGColor(0.1f, 0.1f, 0.1f, 1.0f)
			.DrawDistance(static_cast<GLfloat>(desktop_width))
			.GLVersion(3, 3)
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
    if (glfwGetKey(wnd, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(wnd, GLFW_KEY_W) == GLFW_PRESS)
        app->GetCamera()->MoveForward();
    if (glfwGetKey(wnd, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(wnd, GLFW_KEY_S) == GLFW_PRESS)
        app->GetCamera()->MoveBackward();
    if (glfwGetKey(wnd, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(wnd, GLFW_KEY_A) == GLFW_PRESS)
        app->GetCamera()->MoveLeft();
    if (glfwGetKey(wnd, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(wnd, GLFW_KEY_D) == GLFW_PRESS)
        app->GetCamera()->MoveRight();

    if (!glfwGetKey(wnd, GLFW_KEY_RIGHT) && !glfwGetKey(wnd, GLFW_KEY_D) &&
        !glfwGetKey(wnd, GLFW_KEY_LEFT) && !glfwGetKey(wnd, GLFW_KEY_A) &&
        !glfwGetKey(wnd, GLFW_KEY_UP) && !glfwGetKey(wnd, GLFW_KEY_W) &&
        !glfwGetKey(wnd, GLFW_KEY_DOWN) && !glfwGetKey(wnd, GLFW_KEY_S))
        app->GetCamera()->ResetStep();

    switch (key)
    {
        //case GLFW_KEY_KP_MULTIPLY:
        //    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        //    {
        //        DirectionalLight& light = app->GetDirectionalLight();
        //        GLfloat diffuse_intensity = light.GetDiffuseIntensity();
        //        light.SetDiffuseIntensity(diffuse_intensity + 0.05f);
        //    }
        //    break;
        //case GLFW_KEY_KP_DIVIDE:
        //    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        //    {
        //        DirectionalLight& light = app->GetDirectionalLight();
        //        GLfloat diffuse_intensity = light.GetDiffuseIntensity();
        //        light.SetDiffuseIntensity(diffuse_intensity - 0.05f);
        //    }
        //    break;
        case GLFW_KEY_KP_ADD:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                //app->SetTesselationAlpha(app->GetTesselationAlpha() + 0.01f);
                //app->SetDisplacementFactor(app->GetDisplacementFactor() + 0.01f);
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                //app->SetTesselationAlpha(app->GetTesselationAlpha() - 0.01f);
                //app->SetDisplacementFactor(app->GetDisplacementFactor() - 0.01f);
            }
            break;
        case GLFW_KEY_R:
            if (action == GLFW_PRESS)
                app->GetCamera()->Reset();
            break;
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
            {
                delete app;
                exit(SUCCESS);
            }
            break;
        default:
            break;
    }
}

void MouseScrollCallback(GLFWwindow* wnd, GLdouble x, GLdouble y)
{
    if (y < 0.0f)
        app->GetCamera()->MoveBackward();
    else
        app->GetCamera()->MoveForward();
}

void MouseMoveCallback(GLFWwindow* wnd, GLdouble x, GLdouble y)
{
    app->GetCamera()->MouseInput(x, y);
}

void MouseButtonCallback(GLFWwindow* wnd, GLint button, GLint action, GLint modifiers)
{
    //if (button == GLFW_MOUSE_BUTTON_LEFT)
    //    if (action == GLFW_PRESS)
    //    {
    //        std::pair<GLint, GLint> xy = app->GetMousePos();
    //        app->_left_mb.is_pressed = true;
    //        app->_left_mb.x = static_cast<GLint>(xy.first);
    //        app->_left_mb.y = static_cast<GLint>(xy.second);
    //    }
    //    else if (action == GLFW_RELEASE)
    //        app->_left_mb.is_pressed = false;
}
