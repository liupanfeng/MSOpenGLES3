#version 300 es
precision highp float;

uniform float     iTime;
uniform vec2      iResolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

in vec2     fragCoord;
out vec4    fragColor;

float tile_num = 30.0;
float tile_num1 = 1.0;
void main( void)
{

    vec2 uv = fragCoord / iResolution;
    uv = floor(uv*tile_num) / tile_num;
    fragColor = texture( iChannel1, uv );
}

