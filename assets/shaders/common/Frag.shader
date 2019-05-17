// Frag.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

/* * * * * * * * * * * * * * * * * * * * */

out vec4 gl_Color;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */