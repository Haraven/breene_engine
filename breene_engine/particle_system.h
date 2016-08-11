#pragma once

#include "shader_program.h"
#include "billboard.h"

namespace breene
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

        virtual breene::PartSysUpdateProgram& Init() override;

        breene::PartSysUpdateProgram& SetDeltaTime(GLuint millis);
        breene::PartSysUpdateProgram& SetTime(GLint time);
        breene::PartSysUpdateProgram& SetRandomTextureUnit(GLuint texture_unit);
        breene::PartSysUpdateProgram& SetLauncherLifetime(GLfloat lifespan_secs);
        breene::PartSysUpdateProgram& SetPrimaryParticleLifetime(GLfloat lifespan_secs);
        breene::PartSysUpdateProgram& SetSecondaryParticleLifetime(GLfloat lifespan_secs);
    private:
    };

    class ParticleSystem
    {
    private:
        breene::ParticleSystem& UpdateParticles(GLint millis);
        breene::ParticleSystem& RenderParticles(const glm::mat4 vp, const glm::vec3& cam_pos);
    public:
        ParticleSystem();

        breene::ParticleSystem& Init(const glm::vec3& pos);
        breene::ParticleSystem& Render(GLint delta_millis, const glm::mat4& vp, const glm::vec3& cam_pos);

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
