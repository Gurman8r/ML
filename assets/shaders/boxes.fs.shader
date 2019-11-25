#shader fragment
// Source: https://www.shadertoy.com/view/3sGGDt

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uniform vec4        u_mouse;        // Mouse State
uniform float       u_delta;        // Delta Time
uniform int         u_frame;        // Frame Index
uniform float       u_fps;          // Frame Rate
uniform float       u_time;         // Total Time
uniform vec4        u_color;        // Main Color
uniform sampler2D   u_texture0;     // Main Texture
uniform vec2        u_resolution;   // Display Size

// This is a combination of a semi-random noise gnerator 
// from Art of Code on Youtube - https://www.youtube.com/channel/UCcAlTqd9zID6aNX3TzwxJXg/videos
// a step/HSV effect I got from https://www.shadertoy.com/view/XdyXz3
// and some stuff I did myself (constantly changing noise and the squares)
// it was basically me learning how to work with masks
// it's a mess, but it works

#define S(a,b,t) smoothstep(a,b,t)

vec4 N14(float t) {
    return fract(sin(t*vec4(123., 1024., 3456., 9564.))*vec4(6547., 345., 7799., 1564.));
}

vec4 NN14(float t) {
    return fract(sin(t*vec4(1243., 10224., 13456., 69564.))*vec4(61547., 4345., 27799., 15564.));
}

float N11(float p) {
    float a = fract(p*345.65);
    a += dot(a, a+34.45);
    return fract(a);
}

float N21 (vec2 p) {
    return fract(sin(p.x*122.+p.y*52453.)*244156.); // returns a single randomish number based on a coord input
}

vec4 RoundedBox( in vec2 p, in vec2 b, in float r, in vec4 rgba ) 
{
    vec2 q = abs(p) - b;
    float d = min(max(q.x,q.y),0.0) + length(max(q,0.0));
    d *= 10.;
    return S(.8,.6,d) * rgba;
}

float RoundedBoxInnerMask( in vec2 p, in vec2 b, in float r) 
{
    vec2 q = abs(p) - b;
    float d = min(max(q.x,q.y),0.0) + length(max(q,0.0));
    d *= 10.;
    return S(.8,.6,d);
}

vec4 RoundedBoxRing( in vec2 p, in vec2 b, in float r) 
{
    vec2 q = abs(p) - b;
    float d = (min(max(q.x,q.y),0.0) + length(max(q,0.0)))*4.;
    return S(1.,0.85,d)-S(.4,.25,d)*vec4(0.,0.,0.,1.);
}

vec4 dots(vec2 uv,float t, vec4 rgba, float seed) {
    vec4 b = vec4(0.); // Box
    float bm = 0.; // Box Mask
    vec4 br = vec4(0.); // Box Ring
    float brm = 0.; // Box Ring Mask
    vec2 p = vec2(0.); // Box coords
    float nn = 20.; // Number of boxes
    float pi = 3.141592653;

    for (float n=1.;n<nn;n++) {
        float ti = (t*2.2)+.22*n; // change move time offset for each box so they don't all move at the same time
        // Put a thing in here to change the seed each cycle
        float pitimes = floor((ti - (pi*.5)) / (pi*2.))*nn;
        float pitimet = floor((ti - (pi*1.5)) / (pi*2.))*nn;
        float s1 = seed*n+pitimes;
        float s2 = seed*n+pitimet;
        vec4 rands = (N14(s1)-.5)*5.;
        vec4 randt = (NN14(s2)-.5)*5.;
        float locx = rands.x+(S(0.,1.,sin(ti)*.5+.5)*(randt.x-rands.x));
        float locy = rands.y+(S(0.,1.,sin(ti)*.5+.5)*(randt.y-rands.y));

        p.x = uv.x+(locx*1.8); // Needs to be wider.  should fix to aspect ratio, but cba to learn how right now.
        p.y = uv.y+locy;
        
        b += RoundedBox(vec2(p),vec2(0.5,0.5),0.1, rgba);
        bm += RoundedBoxInnerMask(vec2(p.x,p.y),vec2(0.5,0.5),0.1);
        br += RoundedBoxRing(vec2(p.x,p.y),vec2(0.5,0.5),0.1);
    }
    float maskring = clamp(1.-bm,0.,1.);
    return (b)-(maskring*br);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = ((fragCoord - 0.5).xy * u_resolution.xy) / u_resolution.y;
    
    vec2 m = (u_mouse.xy / u_resolution) * vec2(1, -1);
    float  t = u_time*.6+m.x+6.;
    uv *= 5.;
    vec4 col = vec4(.0);
    
    col += dots(uv,t,vec4(.1,.1,.35,.15),N11(54.)); // Add some boxes
    
    fragColor = vec4(col);
}

void main()
{
    mainImage(gl_Color, V.texcoord);
}