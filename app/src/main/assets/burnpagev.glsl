#version 300 es

layout(location = 0) in vec3 a_position;   //顶点
layout(location = 1) in vec2 a_uv;    //纹理贴图

uniform mat4   u_mat;  //mvp矩阵
out vec2 fragCoord;   //输出的材质顶点

void main(void)
{
    fragCoord = a_uv;
    gl_Position = u_mat * vec4(a_position,1.0);

}