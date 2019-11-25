#shader fragment
// Source: https://www.shadertoy.com/view/3scXzn

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4        u_color;        // Main Color
uniform sampler2D   u_texture0;     // Main Texture
uniform vec2        u_resolution;   // Display Size
uniform float       u_time;         // Total Time

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = ((fragCoord - 0.5).xy * u_resolution.xy) / u_resolution.y;
    uv *= 5.;
    float f = smoothstep(-.4,.4,sin(atan(uv.y, uv.x)*4.-length(uv)*5. +u_time) + sin(length(uv)+u_time));
    f = 1.- abs(f-1.)*10.0;
    fragColor = vec4(f,f,f,1);
}

void main()
{
    mainImage(gl_Color, V.texcoord);
}