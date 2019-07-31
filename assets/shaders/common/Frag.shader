#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

out vec4 gl_Color;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */