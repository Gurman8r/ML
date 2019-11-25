#shader fragment
// Source: https://www.shadertoy.com/view/wsGGDz

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4        u_mouse;        // Mouse State
uniform float       u_delta;        // Delta Time
uniform int         u_frame;        // Frame Index
uniform float       u_fps;          // Frame Rate
uniform float       u_time;         // Total Time
uniform vec4        u_color;        // Main Color
uniform sampler2D   u_texture0;     // Main Texture
uniform vec2        u_resolution;   // Display Size

#define SIN(a) smoothstep(-4., 2.5, sin(a))
#define SIN2(a) (SIN(a)*SIN(a))

vec2 rot(vec2 p, float a)
{
    vec2 i = vec2(cos(a), sin(a));
    return vec2(p.x*i.x - p.y*i.y, p.x*i.y + p.y*i.x);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (2.*(fragCoord - 0.5).xy * u_resolution.xy) / u_resolution.y;

    uv.x += .1*sin(uv.y*uv.x*40.);

    uv += rot(uv, length(uv + u_time));

    vec2 tuv = abs(log(-uv * .005 + 0.909));

    float v = 1.01 - .0005;

    vec3 col = vec3(
        SIN2(20. / pow(tuv.x, v*v*v)),
        SIN2(20. / pow(tuv.x, v*v)),
        SIN2(20. / pow(tuv.x, v))
    ) / length(tuv*7.);

    fragColor = vec4(col, 1.0);
}

void main()
{
    mainImage(gl_Color, V.texcoord);
}