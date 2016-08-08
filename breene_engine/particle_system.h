#pragma once

#include "shader_program.h"
#include "billboard.h"

namespace gl_app
{
    struct Particle
    {
        Particle() {}

        Particle(GLfloat type, const glm::vec3& pos, const glm::vec3& velocity, GLfloat lifespan)
        : type(type)
        , pos(pos)
        , velocity(velocity)
        , lifespan_ms(lifespan)
        {}

        GLfloat type;
        glm::vec3 pos;
        glm::vec3 velocity;
        GLfloat lifespan_ms;
    };

    class PartSysUpdateProgram : public ShaderProgram
    {
    public:
        PartSysUpdateProgram();

        virtual gl_app::PartSysUpdateProgram& Init() override;

        gl_app::PartSysUpdateProgram& SetDeltaTime(GLuint millis);
        gl_app::PartSysUpdateProgram& SetTime(GLint time);
        gl_app::PartSysUpdateProgram& SetRandomTextureUnit(GLuint texture_unit);
        gl_app::PartSysUpdateProgram& SetLauncherLifetime(GLfloat lifespan_secs);
        gl_app::PartSysUpdateProgram& SetPrimaryParticleLifetime(GLfloat lifespan_secs);
        gl_app::PartSysUpdateProgram& SetSecondaryParticleLifetime(GLfloat lifespan_secs);
    private:
    };

    class ParticleSystem
    {
    private:
        gl_app::ParticleSystem& UpdateParticles(GLint millis);
        gl_app::ParticleSystem& RenderParticles(const glm::mat4 vp, const glm::vec3& cam_pos);
    public:
        ParticleSystem();

        gl_app::ParticleSystem& Init(const glm::vec3& pos);
        gl_app::ParticleSystem& Render(GLint delta_millis, const glm::mat4& vp, const glm::vec3& cam_pos);

        ~ParticleSystem();
    private:
        bool _first_call;
        GLuint _crt_vbo;
        GLuint _crt_tfbo;
        GLuint _particle_buffer[2];
        GLuint _transform_feedback[2];
        PartSysUpdateProgram* _particle_program;
        BillboardProgram* _billboard_program;
        Texture1DRandom* _rand_tex;
        Texture2D* _tex;
        GLint _time;
    };
}
