#version 300 es

layout(location = 0) in vec3 a_position;  //可以预加载
layout(location = 1) in vec2 a_uv;

uniform mat4   u_mat;       //矩阵位置变换信息，随时变化 需要render的时候传入
out vec2 o_uv;               //纹理贴图信息，随时变化，需要render的时候传入   这个是输出  传递给片元着色器

void main(void)
{
    o_uv = a_uv;
    gl_Position = u_mat * vec4(a_position,1.0);

}