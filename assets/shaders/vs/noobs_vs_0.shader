// noobs_vs_0.shader
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

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
} Vert;

uniform struct Time_Uniforms
{
	float delta;
	float total;
} Time;

/* * * * * * * * * * * * * * * * * * * * */

mat4 ml_angleAxis(vec3 axis, float angle)
{
	// http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;
	return mat4(
		oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;

	mat4 rot = ml_angleAxis(vec3(0.0, 1.0, 0.0), Time.total);
	mat4 mvp = (Vert.proj * Vert.view * (Vert.model * rot));
	vec4 pos = mvp * vec4(Out.Position, 1.0);
	vec4 off = vec4(0.0, sin(Time.total), 0.0, 1.0);

	gl_Position = pos + off;

	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */