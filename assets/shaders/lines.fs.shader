#shader fragment
// Source: https://www.shadertoy.com/view/wsGGWR

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

vec2 hash(vec2 p) // replace this by something better
{
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise(in vec2 p)
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

    vec2  i = floor(p + (p.x + p.y)*K1);
    vec2  a = p - i + (i.x + i.y)*K2;
    float m = step(a.y, a.x);
    vec2  o = vec2(m, 1.0 - m);
    vec2  b = a - o + K2;
    vec2  c = a - 1.0 + 2.0*K2;
    vec3  h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
    vec3  n = h * h*h*h*vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));
    return dot(n, vec3(70.0));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float rand(float n)
{
    return fract(cos(n*89.42)*343.42);
}

#define barWidth 0.02
#define barRepeat 0.3

vec4 bufferA(vec2 fragCoord)
{
    vec2 uv = (fragCoord * u_resolution) / u_resolution.y;
    float origX = uv.x;
    float val = noise(uv*0.3 + u_time * 0.05 + 0.1)*0.6;
    uv.y += val;
    uv.x += rand(floor(uv.y / barWidth));
    // different speed for each line
    float speed = 0.15 + rand(floor(uv.y / barWidth)*barWidth)*0.1;
    uv.x = mod(uv.x - u_time * speed, barRepeat) / barRepeat;

    vec3 col = mix(vec3(0.4, 0.4, 1.0), vec3(0.4, 1.0, 0.4), origX)*uv.x;
    return vec4(col, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const float blurSize = 1.0 / 256.0;

vec3 getCol(vec2 pos)
{
    return pow(bufferA(pos).rgb, vec3(5.0));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (fragCoord * u_resolution) / u_resolution.y;

    vec3 color = bufferA(uv).rgb;

    vec3 sum = vec3(0);

    sum += getCol(vec2(uv.x - 4.0*blurSize, uv.y)) * 0.05;
    sum += getCol(vec2(uv.x - 3.0*blurSize, uv.y)) * 0.09;
    sum += getCol(vec2(uv.x - 2.0*blurSize, uv.y)) * 0.12;
    sum += getCol(vec2(uv.x - 1.0*blurSize, uv.y)) * 0.15;
    sum += getCol(vec2(uv.x, uv.y)) * 0.16;
    sum += getCol(vec2(uv.x + 1.0*blurSize, uv.y)) * 0.15;
    sum += getCol(vec2(uv.x + 2.0*blurSize, uv.y)) * 0.12;
    sum += getCol(vec2(uv.x + 3.0*blurSize, uv.y)) * 0.09;
    sum += getCol(vec2(uv.x + 4.0*blurSize, uv.y)) * 0.05;

    sum += getCol(vec2(uv.x, uv.y - 4.0*blurSize)) * 0.05;
    sum += getCol(vec2(uv.x, uv.y - 3.0*blurSize)) * 0.09;
    sum += getCol(vec2(uv.x, uv.y - 2.0*blurSize)) * 0.12;
    sum += getCol(vec2(uv.x, uv.y - 1.0*blurSize)) * 0.15;
    sum += getCol(vec2(uv.x, uv.y)) * 0.16;
    sum += getCol(vec2(uv.x, uv.y + 1.0*blurSize)) * 0.15;
    sum += getCol(vec2(uv.x, uv.y + 2.0*blurSize)) * 0.12;
    sum += getCol(vec2(uv.x, uv.y + 3.0*blurSize)) * 0.09;
    sum += getCol(vec2(uv.x, uv.y + 4.0*blurSize)) * 0.05;

    fragColor = vec4(color + sum, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void main()
{
    mainImage(gl_Color, V.texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */