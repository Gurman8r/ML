#shader vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

uniform mat4 u_proj;	// Projection Matrix
uniform mat4 u_view;	// View Matrix
uniform mat4 u_model;	// Model Matrix

void main()
{
	V.position = a_position;
	V.normal = a_normal;
	V.texcoord = a_texcoord;
	gl_Position = (u_proj * u_view * u_model) * vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Source: https://www.shadertoy.com/view/wsGGDz

#version 460 core

in Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

out vec4 gl_Color;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uniform vec2		u_cursor;		// Cursor Position
uniform float		u_delta;		// Delta Time
uniform int			u_frame;		// Frame Index
uniform float		u_fps;			// Frame Rate
uniform float		u_time;			// Total Time
uniform	vec4		u_color;		// Main Color
uniform sampler2D	u_texture0;		// Main Texture
uniform vec2 		u_viewport;		// Viewport Size

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define iFrame		u_frame
#define iMouse		u_cursor
#define iResolution u_viewport
#define iTime		u_time
#define iTimeDelta	u_delta

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SIN(a) smoothstep(-4., 2.5, sin(a))
#define SIN2(a) (SIN(a)*SIN(a))

vec2 rot(vec2 p, float a)
{
	vec2 i = vec2(cos(a), sin(a));
	return vec2(p.x*i.x - p.y*i.y, p.x*i.y + p.y*i.x);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = (2.*fragCoord * iResolution.xy) / iResolution.y;

	uv.x += .1*sin(uv.y*uv.x*40.);

	uv += rot(uv, length(uv + iTime));

	vec2 tuv = abs(log(-uv * .005 + 0.909));

	float v = 1.01 - .0005;

	vec3 col = vec3(
		SIN2(20. / pow(tuv.x, v*v*v)),
		SIN2(20. / pow(tuv.x, v*v)),
		SIN2(20. / pow(tuv.x, v))
	) / length(tuv*7.);

	fragColor = vec4(col, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void main()
{
	mainImage(gl_Color, V.texcoord - vec2(0.5));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */