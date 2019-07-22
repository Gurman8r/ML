// u_shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

/* * * * * * * * * * * * * * * * * * * * */

out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */