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
#include "basic_shader_programs.h"
#include "my_constants.h"

namespace breene
{
	enum Stats
	{
		DISPLAY_NONE   = 0x00,
		DISPLAY_FPS    = 0x01,
		DISPLAY_UPTIME = 0x02
	};

	class ApplicationBuilder;

    class BreeneApplication
    {
    private:
        const static size_t ROWCOUNT = 50, COLCOUNT = 2;
        const static size_t INSTANCE_COUNT = ROWCOUNT * COLCOUNT;

        void CalcPositions();
        GLfloat CalcUptime();
        void CalcFPS();
		void RenderFPS();

		void Init();
		void InitLights();
		void InitPositions();
		void DeallocateResources();

        void ShadowMapPass();
        void PickingPass();
        void RenderPass();

		GLfloat CalcPointLightSphere(const PointLight& light);
		GLfloat CalcSpotLightSphere(const SpotLight& light);
        void DeferredShadingGeometryPass();
		void DefShadingStencilPass(GLuint index);
		void DefShadingPointLightPass(GLuint index);
		void DefShadingFinalPass();
		void DeferredShadingDirLightPass();
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
		static breene::ApplicationBuilder& GetBuilder();

        /*GLfloat GetDisplacementFactor() const { return _displacement_factor; }
        GLfloat GetTesselationLevel() const { return _tess_level; }
        GLfloat GetTesselationAlpha() const { return _tess_alpha; }*/

        RetCodes MakeWindow(GLchar* title, GLenum is_fullscreen, GLint sampling, GLint openGL_version_major, GLint openGL_version_minor, GLint openGL_profile, GLenum capture_input, GLenum hide_cursor = GL_FALSE, GLenum depth_test = GL_FALSE);

        breene::BreeneApplication& SetWindowWidth(GLulong width);
        breene::BreeneApplication& SetWindowHeight(GLulong height);
        breene::BreeneApplication& SetBackgroundColor(const glm::vec4& rgba);
        breene::BreeneApplication& SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        breene::BreeneApplication& SetCamera(Camera* camera);
        breene::BreeneApplication& SetPerspectiveZFAR(GLfloat z_far);
        breene::BreeneApplication& SetPerspectiveFOV(GLfloat fov);
        breene::BreeneApplication& SetErrorCallback(const ErrorCallbackFn& callback);
        breene::BreeneApplication& SetKeyCallback(const KeyCallbackFn& callback);
        breene::BreeneApplication& SetCursorCallback(const MouseCallbackFn& callback);
        breene::BreeneApplication& SetMouseButtonCallback(const GLFWmousebuttonfun& callback);
        breene::BreeneApplication& SetMouseScrollCallback(const MouseCallbackFn& callback);
        breene::BreeneApplication& ToggleStatsDisplay();
		breene::BreeneApplication& SetStatsToDisplay(uint8_t stats);
		breene::BreeneApplication& SetVsync(GLenum on_off);
        //gl_app::OpenGLApplication& SetDisplacementFactor(GLfloat displacement_factor);
        //gl_app::OpenGLApplication& SetTesselationLevel(const GLfloat level);
        //gl_app::OpenGLApplication& SetTesselationAlpha(const GLfloat alpha);
        std::pair<GLint, GLint> GetMousePos();
        Camera* GetCamera() const { return _camera; }

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
		BlankProgram* _blank_program;
        DefShadingGeomProgram* _deferred_shading_geometry_program;
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
        //ParticleSystem* _particle_system;
        DirectionalLight _dir_light;
		PointLight _point_lights[5];
		SpotLight _spot_light;
        transform::PerspectiveProjectionInfo _perspective_info;
        glm::vec3 _positions[INSTANCE_COUNT];
        GLfloat _velocities[INSTANCE_COUNT];
        glm::vec4 _clear_color;
        GLfloat _scale;
		uint8_t _stats_to_display;
        bool _display_stats;
        GLfloat _fps;
        long long _frametime;
        long long _start_time;
        //long long _crt_time_ms;
    };

	class ApplicationBuilder
	{
	public:
		enum StartupParams
		{
			VSYNC         = 0x01,
			FULLSCREEN    = 0x02,
			CAPTURE_INPUT = 0x04,
			HIDE_CURSOR   = 0x08,
			DEPTH_TEST    = 0x10
		};

		ApplicationBuilder();

		breene::ApplicationBuilder& Title(GLchar* title);
		breene::ApplicationBuilder& Width(GLulong width);
		breene::ApplicationBuilder& Height(GLulong height);
		breene::ApplicationBuilder& BGColor(glm::vec4& color);
		breene::ApplicationBuilder& BGColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		breene::ApplicationBuilder& Cam(Camera* camera);
		breene::ApplicationBuilder& DrawDistance(GLfloat units);
		breene::ApplicationBuilder& MSAA(GLuint samples);
		breene::ApplicationBuilder& FOV(GLfloat fov);
		breene::ApplicationBuilder& GLVersion(GLint major, GLint minor);
		breene::ApplicationBuilder& StatsToDisplay(uint8_t stats);
		breene::ApplicationBuilder& Vsync(bool on_off);
		breene::ApplicationBuilder& Fullscreen(bool on_off);
		breene::ApplicationBuilder& CaptureInput(bool on_off);
		breene::ApplicationBuilder& HideCursor(bool on_off);
		breene::ApplicationBuilder& DepthTest(bool on_off);
		BreeneApplication* Build();

	private:
		glm::vec4 _bg_color;
		GLulong _width;
		GLulong _height;
		Camera* _camera;
		GLfloat _draw_dist;
		GLfloat _fov;
		GLuint _samples;
		GLint _gl_ver_major;
		GLint _gl_ver_minor;
		GLchar* _title;
		uint8_t _display_stats;
		uint8_t _startup_params;
	};
}