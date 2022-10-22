#version 300 es
precision highp float;

uniform float     iTime;
uniform vec2      iResolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

in vec2     fragCoord;
out vec4    fragColor;

void main( void)
{
    vec2 uv = fragCoord;

    vec3 col = vec3(0.);

    vec3 heightmap =  texture(iChannel1, uv).rrr;
    vec3 background = texture(iChannel1, uv).rgb;
    vec3 foreground = texture(iChannel1, uv).rgb;

    float t = fract(-iTime*.2);
    vec3 erosion = smoothstep(t-.2, t, heightmap);

    vec3 border = smoothstep(0., .1, erosion) - smoothstep(.1, 1., erosion);

    col = (1.-erosion)*foreground + erosion*background;

    vec3 leadcol = vec3(1., .5, .1);
    vec3 trailcol = vec3(0.2, .4, 1.);
    vec3 fire = mix(leadcol, trailcol, smoothstep(0.8, 1., border))*2.;

    col += border*fire;

    fragColor = vec4(col,1.0);
}
