#version 330

const int PARTICLE_TYPE_LAUNCHER = 0;
const int PARTICLE_TYPE_PRIMARY_SHELL = 1;
const int PARTICLE_TYPE_SECONDARY_SHELL = 2;
const float VELOCITY_CONST = 10.0f;
const int SECONDARY_PARTICLE_COUNT = 10;

layout (points) in;
layout (points) out;
layout (max_vertices = 30) out;

in float type_0[];
in vec3 pos_0[];
in vec3 vel_0[];
in float age_0[];

out float type_1;
out vec3 pos_1;
out vec3 vel_1;
out float age_1;

uniform float g_delta_time;
uniform float g_time;
uniform sampler1D g_rand_tex;
uniform float g_launcher_lifespan;
uniform float g_primary_particle_lifespan;
uniform float g_secondary_particle_lifespan;

vec3 GetRandomDirection(float tex_coord)
{
    return texture(g_rand_tex, tex_coord).xyz - vec3(0.5f);
}

void main()
{
    float age = age_0[0] + g_delta_time;

    if (type_0[0] == PARTICLE_TYPE_LAUNCHER)
    {
        if (age >= g_launcher_lifespan)
        {
            type_1 = PARTICLE_TYPE_PRIMARY_SHELL;
            pos_1 = pos_0[0];
            vec3 dir = GetRandomDirection(g_time / 1000.0f);
            dir.y = max(dir.y, 0.5f);
            vel_1 = normalize(dir) / VELOCITY_CONST;
            age_1 = 0.0f;
            EmitVertex();
            EndPrimitive();
            age = 0.0f;
        }

        type_1 = PARTICLE_TYPE_LAUNCHER;
        pos_1 = pos_0[0];
        vel_1 = vel_0[0];
        age_1 = age;
        EmitVertex();
        EndPrimitive();
    }
    else
    {
        float delta_secs = g_delta_time / 1000.0f;
        float t1 = age_0[0] / 1000.0f;
        float t2 = age / 1000.0f;
        vec3 delta_p = delta_secs * vel_0[0];
        vec3 delta_v = vec3(delta_secs) * (0.0f, -9.81f, 0.0f);

        if (type_0[0] == PARTICLE_TYPE_PRIMARY_SHELL)
        {
	        if (age < g_primary_particle_lifespan) 
            {
	            type_1 = PARTICLE_TYPE_PRIMARY_SHELL;
	            pos_1 = pos_0[0] + delta_p;
	            vel_1 = vel_0[0] + delta_v;
	            age_1 = age;
	            EmitVertex();
	            EndPrimitive();
	        }
            else
            {
                for (int i = 0; i < SECONDARY_PARTICLE_COUNT; ++i)
                {
                     type_1 = PARTICLE_TYPE_SECONDARY_SHELL;
                     pos_1 = pos_0[0];
                     vec3 dir = GetRandomDirection((g_time + i) / 1000.0f);
                     vel_1 = normalize(dir) / VELOCITY_CONST;
                     age_1 = 0.0f;
                     EmitVertex();
                     EndPrimitive();
                }
            }
        }
        else
        {
            if (age < g_secondary_particle_lifespan)
            {
                type_1 = PARTICLE_TYPE_SECONDARY_SHELL;
                pos_1 = pos_0[0] + delta_p;
                vel_1 = vel_0[0] + delta_v;
                age_1 = age;
                EmitVertex();
                EndPrimitive();
            }
        }
    }
}
