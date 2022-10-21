#version 300 es

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4   u_mat;
out vec4   o_color;

void main(void)
{
    o_color = a_color;
    gl_Position = u_mat * a_position;

}