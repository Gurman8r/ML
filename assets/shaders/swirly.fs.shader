#shader fragment
// Source: https://www.shadertoy.com/view/wdG3Wh

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

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = ((fragCoord - 0.5).xy * u_resolution.xy) / u_resolution.y;
    uv = vec2(noise(uv + u_time * 0.1), noise(uv + 10.));

    float d = uv.x - uv.y;
    d *= 20.;
    d = sin(d);
    d = d * 0.5 + 0.5;
    d = 1.0 - d;
    d = smoothstep(0.1, 0.1, d);

    vec3 col = vec3(mix(vec3(0.1), vec3(0.2, 0.2, 0.6), d));

    fragColor = vec4(col, 1.0);
}

void main()
{
    mainImage(gl_Color, V.texcoord);
}