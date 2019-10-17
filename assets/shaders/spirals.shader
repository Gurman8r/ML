#shader vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

uniform mat4	u_proj;		// Projection Matrix
uniform mat4	u_view;		// View Matrix
uniform mat4	u_model;	// Model Matrix

void main()
{
	V.position	= a_position;
	V.normal	= a_normal;
	V.texcoord	= a_texcoord;
	gl_Position = (u_proj * u_view * u_model) * vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Source: https://www.shadertoy.com/view/3scXzn

#version 460 core

in Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform	vec4		u_color;	// Main Color
uniform sampler2D	u_texture0;	// Main Texture
uniform vec2 		u_viewport;	// Display Size
uniform float 		u_time;		// Total Time

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord.xy * u_viewport.xy) / u_viewport.y;
	uv *= 5.;
    float f = smoothstep(-.4,.4,sin(atan(uv.y, uv.x)*4.-length(uv)*5. +u_time) + sin(length(uv)+u_time));
    f = 1.- abs(f-1.)*10.0;
    fragColor = vec4(f,f,f,1);
}

void main()
{
	mainImage(gl_Color, V.texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */