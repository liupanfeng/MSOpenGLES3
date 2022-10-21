#version 300 es
precision mediump float;

uniform sampler2D utexture;
in vec2 o_uv;
out vec4  fragColor;

void main(void)
{
    fragColor = texture(utexture,o_uv);
}
