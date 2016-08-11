#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include <vector>
#include <functional>
#include "utils.h"
#include "camera.h"
#include "mesh.h"
#include "lighting.h"
#include "shadows.h"
#include "skybox.h"
#include "billboard.h"
#include "particle_system.h"
#include "basic_coloring.h"
#include "3d_picking.h"
#include "text_rendering.h"
#include "deferred_shading.h"
#include "my_constants.h"

namespace breene
{
    class BreeneApplication
    {
    private:
        const static size_t ROWCOUNT = 50, COLCOUNT = 20;
        const static size_t INSTANCE_COUNT = ROWCOUNT * COLCOUNT;

        void CalcPositions();
        void CalcFPS();
        GLfloat CalcUptime();
        void DeallocateResources();
        void Init();

        void ShadowMapPass();
        void PickingPass();
        void RenderPass();
        void DeferredShadingGeometryPass();
        void DeferredShadingLightPass();
    public:
        typedef GLFWkeyfun KeyCallbackFn;
        typedef GLFWcursorposfun MouseCallbackFn;
        typedef GLFWerrorfun ErrorCallbackFn;
        typedef GLFWmousebuttonfun MouseButtonCallbackFn;
        typedef std::function<void(void)> DisplayManipFn;
        typedef std::function<void(GLuint)> UniformHandlerFn;

        BreeneApplication();
        BreeneApplication(GLulong window_width, GLulong window_height);
        BreeneApplication(GLulong window_width, GLulong window_height, Camera* camera);
                
        breene::BreeneApplication& Run();
        
        /*GLfloat GetDisplacementFactor() const { return _displacement_factor; }
        GLfloat GetTesselationLevel() const { return _tess_level; }
        GLfloat GetTesselationAlpha() const { return _tess_alpha; }*/

        RetCodes MakeWindow(GLchar* title, GLenum is_fullscreen, GLint sampling, GLint openGL_version_major, GLint openGL_version_minor, GLint openGL_profile, GLenum capture_input, GLenum hide_cursor = GL_FALSE, GLenum depth_test = GL_FALSE);
        breene::BreeneApplication& SetWindowWidth(GLulong width);
        breene::BreeneApplication& SetWindowHeight(GLulong height);
        breene::BreeneApplication& SetBackgroundColor(const glm::vec4& rgba);
        breene::BreeneApplication& SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        breene::BreeneApplication& SetCamera(Camera* camera);
        //gl_app::OpenGLApplication& SetDisplacementFactor(GLfloat displacement_factor);
        breene::BreeneApplication& SetPerspectiveZNear(GLfloat z_near);
        breene::BreeneApplication& SetPerspectiveZFAR(GLfloat z_far);
        breene::BreeneApplication& SetPerspectiveFOV(GLfloat fov);
        breene::BreeneApplication& SetErrorCallback(const ErrorCallbackFn& callback);
        breene::BreeneApplication& SetKeyCallback(const KeyCallbackFn& callback);
        breene::BreeneApplication& SetCursorCallback(const MouseCallbackFn& callback);
        breene::BreeneApplication& SetMouseButtonCallback(const GLFWmousebuttonfun& callback);
        breene::BreeneApplication& SetMouseScrollCallback(const MouseCallbackFn& callback);
        breene::BreeneApplication& ToggleStatsDisplay();
        //gl_app::OpenGLApplication& SetTesselationLevel(const GLfloat level);
        //gl_app::OpenGLApplication& SetTesselationAlpha(const GLfloat alpha);
        std::pair<GLint, GLint> GetMousePos();
        Camera* GetCamera() const { return _camera; }
        DirectionalLight _dir_light;
        //struct MouseButton
        //{
        //    MouseButton() {}
        //    MouseButton(bool is_pressed, GLint x, GLint y)
        //    : is_pressed(is_pressed)
        //    , x(x)
        //    , y(y)
        //    {}

        //    bool is_pressed;
        //    GLint x;
        //    GLint y;
        //};
        //MouseButton _left_mb;

        ~BreeneApplication();
    private:

        GLFWwindow* _wnd;
        GLulong _wnd_width;
        GLulong _wnd_height;
        Camera* _camera;
        Mesh* _mesh;
        /*Mesh* _ground;
        Texture2D* _ground_tex;
        Texture2D* _ground_tex_normal_map;*/
        //Texture2D* _mesh_tex;
        //ShadowProgram* _shadowmap_program;
        DefShadingGeomProgram* _deferred_shading_program;
        GeometryBuffer* _geometry_buffer;
        LightingProgram* _lighting_program;
        text_rendering::TextRenderer* _text_renderer;
        text_rendering::TextureAtlas* _verdana;
        text_rendering::TextureAtlas* _arial;
        //Texture2D* _displacement_map;
        //Texture2D* _color_map;
        //Texture2D* _normal_map;
        //GLfloat _displacement_factor;
        //GLfloat _tess_level;
        //GLfloat _tess_alpha;
        //bool _is_wireframe;
        //PickingProgram* _picking_program;
        //PlainColorProgram* _plain_program;
        //PickingFBO* _picking_fbo;
        //SkyBox* _skybox;
        //ShadowMapFBO* _shadow_fbo;  
        //SpotLight _spot_light;
        //ParticleSystem* _particle_system;
        transform::PerspectiveProjectionInfo _perspective_info;
        //glm::vec3 _world_pos[2];
        glm::vec3 _positions[INSTANCE_COUNT];
        GLfloat _velocities[INSTANCE_COUNT];
        glm::vec4 _clear_color;
        GLfloat _scale;
        bool _display_stats;
        GLfloat _fps;
        long long _frametime;
        long long _start_time;
        //long long _crt_time_ms;
    };
}