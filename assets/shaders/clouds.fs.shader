#shader fragment
// Source: https://www.shadertoy.com/view/3sGGDt

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

// This is a combination of a semi-random noise gnerator 
// from Art of Code on Youtube - https://www.youtube.com/channel/UCcAlTqd9zID6aNX3TzwxJXg/videos
// a step/HSV effect I got from https://www.shadertoy.com/view/XdyXz3
// and some stuff I did myself (constantly changing noise and the squares)
// it was basically me learning how to work with masks
// it's a mess, but it works

#define S(a,b,t) smoothstep(a,b,t)
const float LEVELS = 12.;

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

vec3 hsv2rgb(vec3 c){  // taken from https://www.shadertoy.com/view/XdyXz3
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float SmoothNoise(vec2 uv) {
     vec2 lv = fract(uv);
    vec2 id = floor(uv);
    
    lv = lv*lv*(3.-2.*lv);
    
    float bl = N21(id);
    float br = N21(id+vec2(1,0));
    float b = mix(bl, br, lv.x);
    float tl = N21(id+vec2(0,1));
    float tr = N21(id+vec2(1,1));
    float t = mix(tl, tr, lv.x);
    
    return mix(b,t,lv.y);
}

float SmoothNoise2(vec2 uv,float d) {
    float c;
    float uvscale = 4.;
    float detail = 1.;
    for (float i=1.;i<=d;i++) {
        c += SmoothNoise(uv*uvscale)*detail;
        detail *= 0.5;
        uvscale *= 2.;
    }
    return c/1.3;;
}

float GetClouds(vec2 uv, float t, float d) {
    float speed = sin(t)*.5+.5;
    // put a thing in here so that whenever the mix is fully one way or another, the 'hidden' one changes
    float pitime = mod(floor((t - (3.141*.5)) / 6.282),100.);
    float pitime2 = mod(floor((t - (3.141*1.5)) / 6.282),100.);
    vec2 offs = vec2(-2.,-2.);
    vec2 target1 = uv+(offs*pitime);
    vec2 target2 = uv+vec2(11.,22.)+(offs*pitime2);
    
    // pull two noise maps
    float c1 = SmoothNoise2(target1, d);
    float c2 = SmoothNoise2(target2, d);
    float c = mix(c1,c2,speed);                 
    
    return floor(c*LEVELS)/LEVELS;
}

vec3 DoClouds (vec2 uv) {
    uv *= .2;
    float t = u_time*1.;
    //vec2 offset = vec2(sin(t)*0.006,cos(t)*0.006);  // rotating lightsource. doesn't look so good without a visual light cue.
    vec2 offset = vec2(0.006,0.006);
    
    uv += u_time*0.02;  // slow diagonal move
    float detail = 4.; // 1-6 working range

    float c = GetClouds(uv,t,detail);
    float edge = GetClouds(uv+offset,t,detail);
    
    //calc HSV color (taken from https://www.shadertoy.com/view/XdyXz3)
    float h = c*.9 - .2; //rainbow hue
    float s = 0.5;
    float v = 1. - ( c*(detail*.05) ); //deeper is darker
    
    v -= c-edge;
    
    return hsv2rgb(vec3(-h,s,v));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = ((fragCoord - 0.5).xy * u_resolution.xy) / u_resolution.y;
    
    uv *= 5.;
    
    vec4 col = vec4(.0);
    
    col += vec4(DoClouds(uv),1.);
    
    fragColor = vec4(col);
}

void main()
{
    mainImage(gl_Color, V.texcoord);
}