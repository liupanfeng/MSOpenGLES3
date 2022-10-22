#version 300 es
precision mediump float;

uniform float     uValue;    //这个是变量  需要在render的时候传递

uniform sampler2D utexture0;    //2d 纹理采样器
uniform sampler2D utexture1;

in vec2 o_uv;                 //顶点着色器传递过来的参数
out vec4  fragColor;           //这个是输出到下一个阶段的输出    这个命名不能变化  否则下一个阶段接收不到

void main(void)
{
    //fragColor = texture(utexture,o_uv);
    vec4 newColor = vec4(0,0,0,0);

    if(o_uv.x > (1.0-uValue)){
        newColor = texture(utexture1,vec2(o_uv.x - (1.0f - uValue),  o_uv.y  )) ;
    }
    else{
        newColor = texture(utexture0,o_uv);
    }

    fragColor = newColor;
}
