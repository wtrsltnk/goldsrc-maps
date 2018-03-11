#version 150

in vec3 vertex;
in vec3 uv;

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

out vec2 f_uv;

void main()
{
    // Position
    gl_Position = u_p * u_v * u_m * vec4(vertex.xyz, 1.0);

    // Texcoords
    f_uv = uv.xy;
}
