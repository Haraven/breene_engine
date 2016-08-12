#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <functional>
#include "lighting.h"
#include "camera.h"
#include "mesh.h"
#include "deferred_shading.h"
#include "text_rendering.h"
#include "utils.h"
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
		void InitLights();
		void InitPositions();

        void ShadowMapPass();
        void PickingPass();
        void RenderPass();

		GLfloat CalcPointLightSphere(const PointLight& light);
		GLfloat CalcSpotLightSphere(const SpotLight& light);
        void DeferredShadingGeometryPass();
		void DeferredShadingSetupLights();
		void DeferredShadingSpotLightsPass();
        void DeferredShadingPointLightsPass();
		void DeferredShadingDirLightPass();
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
		Mesh* _quad;
		Mesh* _sphere;
        /*Mesh* _ground;
        Texture2D* _ground_tex;
        Texture2D* _ground_tex_normal_map;*/
        //Texture2D* _mesh_tex;
        //ShadowProgram* _shadowmap_program;
        DefShadingGeomProgram* _deferred_shading_program;
		DefShadingDirLight* _dir_light_program;
		DefShadingPointLight* _pt_light_program;
		DefShadingSpotLight* _spot_light_program;
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
		PointLight _point_lights[5];
		SpotLight _spot_light;
        transform::PerspectiveProjectionInfo _perspective_info;
        glm::vec3 _positions[5];
        //GLfloat _velocities[INSTANCE_COUNT];
        glm::vec4 _clear_color;
        GLfloat _scale;
        bool _display_stats;
        GLfloat _fps;
        long long _frametime;
        long long _start_time;
        //long long _crt_time_ms;
    };
}