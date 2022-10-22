#version 300 es

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

uniform mat4   u_mat;
out vec2 fragCoord;

void main(void)
{
    fragCoord = a_uv;
    gl_Position = u_mat * vec4(a_position,1.0);

}