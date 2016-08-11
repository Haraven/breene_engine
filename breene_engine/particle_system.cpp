#include "particle_system.h"
#include <iostream>
#include "my_constants.h"

const GLuint MAX_PARTICLES = 1000;
const GLfloat PARTICLE_LIFESPAN = 10.0f;

enum ParticleType
{
    PARTICLE_TYPE_LAUNCHER = 0,
    PARTICLE_TYPE_PRIMARY_SHELL,
    PARTICLE_TYPE_SECONDARY_SHELL
};

breene::PartSysUpdateProgram::PartSysUpdateProgram()
{}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::Init()
{
    const size_t VARYINGS_COUNT = 4;

    ShaderProgram::Init();

    AddShader(Shader(PARTSYSUPDATE_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(PARTSYSUPDATE_GEOMETRY_SHADER, GL_GEOMETRY_SHADER));
    AddShader(Shader(PARTSYSUPDATE_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));

    const GLchar* varyings[VARYINGS_COUNT] = 
    {
        "type_1",
        "pos_1",
        "vel_1",
        "age_1"
    };

    glTransformFeedbackVaryings(_program, VARYINGS_COUNT, varyings, GL_INTERLEAVED_ATTRIBS);

    Finalize();

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetDeltaTime(GLuint millis)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint delta_time_uniform_loc = GetUniformLocation(PARTSYSUPDATE_DELTA_TIME_UNIFORM);
    glUniform1f(delta_time_uniform_loc, static_cast<GLfloat>(millis));

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetTime(GLint time)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint time_uniform_loc = GetUniformLocation(PARTSYSUPDATE_TIME_UNIFORM);
    glUniform1f(time_uniform_loc, static_cast<GLfloat>(time));

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetRandomTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint rand_tex_uniform_loc = GetUniformLocation(RANDTEX_UNIFORM);
    glUniform1i(rand_tex_uniform_loc, texture_unit);

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetLauncherLifetime(GLfloat lifespan_secs)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint launcher_lifespan_uniform_loc = GetUniformLocation(PARTSYSUPDATE_LAUNCHER_LIFESPAN_UNIFORM);
    glUniform1f(launcher_lifespan_uniform_loc, lifespan_secs);

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetPrimaryParticleLifetime(GLfloat lifespan_secs)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint primary_particle_lifespan_uniform_loc = GetUniformLocation(PARTSYSUPDATE_PARTICLEI_LIFESPAN_UNIFORM);
    glUniform1f(primary_particle_lifespan_uniform_loc, lifespan_secs);

    return *this;
}

breene::PartSysUpdateProgram & breene::PartSysUpdateProgram::SetSecondaryParticleLifetime(GLfloat lifespan_secs)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint secondary_particle_uniform_loc = GetUniformLocation(PARTSYSUPDATE_PARTICLEII_LIFESPAN_UNIFORM);
    glUniform1f(secondary_particle_uniform_loc, lifespan_secs);

    return *this;
}

breene::ParticleSystem & breene::ParticleSystem::UpdateParticles(GLint millis)
{
    const GLuint TYPE_INDEX = 0,
        POS_INDEX           = 1,
        VEL_INDEX           = 2,
        LIFESPAN_INDEX      = 3;

    _particle_program->Use();
    _particle_program->SetTime(_time)
        .SetDeltaTime(millis);

    _tex->Bind(RANDOM_TEXTURE_UNIT);    

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, _particle_buffer[_crt_vbo]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transform_feedback[_crt_tfbo]);

    glEnableVertexAttribArray(TYPE_INDEX);
    glEnableVertexAttribArray(POS_INDEX);
    glEnableVertexAttribArray(VEL_INDEX);
    glEnableVertexAttribArray(LIFESPAN_INDEX);

    GLvoid* TYPE_OFFSET     = 0; // beginning of array
    GLvoid* POS_OFFSET      = (GLvoid*)sizeof(GLfloat); // 1 float after beginning
    GLvoid* VEL_OFFSET      = (GLvoid*)(sizeof(GLfloat) * 4); // 1 float (type) + 3 floats (pos) after beginning
    GLvoid* LIFESPAN_OFFSET = (GLvoid*)(sizeof(GLfloat) * 7); // 1 float + 3 floats + 3 floats (velocity) after beginning

    glVertexAttribPointer(TYPE_INDEX,     1, GL_FLOAT, GL_FALSE, sizeof(Particle), TYPE_OFFSET);
    glVertexAttribPointer(POS_INDEX,      3, GL_FLOAT, GL_FALSE, sizeof(Particle), POS_OFFSET);
    glVertexAttribPointer(VEL_INDEX,      3, GL_FLOAT, GL_FALSE, sizeof(Particle), VEL_OFFSET);
    glVertexAttribPointer(LIFESPAN_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), LIFESPAN_OFFSET);

    glBeginTransformFeedback(GL_POINTS);
    if (_first_call)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        _first_call = false;
    }
    else
        glDrawTransformFeedback(GL_POINTS, _transform_feedback[_crt_vbo]);
    glEndTransformFeedback();

    glDisableVertexAttribArray(TYPE_INDEX);
    glDisableVertexAttribArray(POS_INDEX);
    glDisableVertexAttribArray(VEL_INDEX);
    glDisableVertexAttribArray(LIFESPAN_INDEX);

    return *this;
}

breene::ParticleSystem & breene::ParticleSystem::RenderParticles(const glm::mat4 vp, const glm::vec3 & cam_pos)
{
    _billboard_program->Use();
    _billboard_program->SetCamPos(cam_pos)
        .SetVP(vp);

    _tex->Bind(COLOR_TEXTURE_UNIT);

    glDisable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, _particle_buffer[_crt_tfbo]);

    GLvoid* POS_OFFSET = (GLvoid*)sizeof(GLfloat); // 1 float after beginning

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), POS_OFFSET);

    glDrawTransformFeedback(GL_POINTS, _transform_feedback[_crt_tfbo]);

    glDisableVertexAttribArray(0);

    return *this;
}

breene::ParticleSystem::ParticleSystem()
: _crt_vbo(0)
, _crt_tfbo(1)
, _first_call(true)
, _time(0)
, _tex(nullptr)
, _particle_program(nullptr)
, _billboard_program(nullptr)
{
    memset(_particle_buffer, 0, sizeof(_particle_buffer));
    memset(_transform_feedback, 0, sizeof(_transform_feedback));
}

breene::ParticleSystem & breene::ParticleSystem::Init(const glm::vec3 & pos)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const size_t LAUNCHER_INDEX = 0;

    Particle particles[MAX_PARTICLES];
    memset(particles, 0, sizeof(particles));

    particles[LAUNCHER_INDEX].type = PARTICLE_TYPE_LAUNCHER;
    particles[LAUNCHER_INDEX].pos = pos;
    particles[LAUNCHER_INDEX].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[LAUNCHER_INDEX].lifespan_ms = 0.0f;

    glGenTransformFeedbacks(2, _transform_feedback);
    glGenBuffers(2, _particle_buffer);

    for (GLuint i = 0; i < 2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transform_feedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _particle_buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _particle_buffer[i]);
    }

    if (_particle_program == nullptr)
        _particle_program = new PartSysUpdateProgram();
    _particle_program->Init().Use();

    _particle_program->SetRandomTextureUnit(RANDOM_TEXTURE_UNIT_INDEX)
        .SetLauncherLifetime(100.0f)
        .SetPrimaryParticleLifetime(10000.0f)
        .SetSecondaryParticleLifetime(2500.0f);

    if (_rand_tex == nullptr)
        _rand_tex = new Texture1DRandom(1000);
    _rand_tex->SetSize(1000)
        .Load()
        .Bind(RANDOM_TEXTURE_UNIT);
    
    if (_billboard_program == nullptr)
        _billboard_program = new BillboardProgram();
    _billboard_program->Init().Use();
    _billboard_program->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX)
        .SetSize(0.01f);

    if (_tex == nullptr)
        _tex = new Texture2D("fireworks_red.jpg", GL_TEXTURE_2D);
    _tex->Load();

    auto res = glGetError();
    if (res != GL_NO_ERROR && res != GL_INVALID_ENUM) throw std::runtime_error("Error initializing particle system. Error code: " + res);

    return *this;
}

breene::ParticleSystem & breene::ParticleSystem::Render(GLint delta_millis, const glm::mat4 & vp, const glm::vec3 & cam_pos)
{
    _time += delta_millis;

    UpdateParticles(delta_millis);
    RenderParticles(vp, cam_pos);

    _crt_vbo = _crt_tfbo;
    _crt_tfbo = (_crt_tfbo + 1) & 0x1;

    return *this;
}

breene::ParticleSystem::~ParticleSystem()
{
    if (_particle_program != nullptr)
    {
        delete _particle_program;
        _particle_program = nullptr;
    }
    if (_billboard_program != nullptr)
    {
        delete _billboard_program;
        _billboard_program = nullptr;
    }
    if (_tex != nullptr)
    {
        delete _tex;
        _tex = nullptr;
    }

    memset(_particle_buffer, 0, sizeof(GLuint) * 2);
    memset(_transform_feedback, 0, sizeof(GLuint) * 2);
}
