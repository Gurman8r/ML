// noobs_vs_0.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Time_Uniforms
{
	float delta;
	float total;
} Time;

uniform struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
} Vert;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;

	gl_Position = 
		(Vert.proj * Vert.view * Vert.model) *
		vec4(Out.Position, 1.0) +
		vec4(0.0, sin(Time.total), 0.0, 1.0);

	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */