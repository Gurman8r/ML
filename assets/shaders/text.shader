// text.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out Vertex {
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

mat4 orthographic(float left, float right, float bottom, float top)
{
	mat4 temp = mat4(1.0);
	temp[0][0] = 2.0 / (right - left);
	temp[1][1] = 2.0 / (top - bottom);
	temp[2][2] = -1;
	temp[3][0] = -(right + left) / (right - left);
	temp[3][1] = -(top + bottom) / (top - bottom);
	return temp;
}

uniform vec2 u_viewport;

void main()
{
	V.Position = a_Position;
	V.Normal = a_Normal;
	V.Texcoord = a_Texcoord;

	mat4 p = orthographic(
		0.0, u_viewport.x,
		0.0, u_viewport.y
	);

	gl_Position = p * vec4(V.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

in Vertex {
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

out vec4 gl_Color;

uniform	vec4		u_color;
uniform sampler2D	u_texture0;

void main()
{
	gl_Color = u_color * vec4(1, 1, 1, texture(u_texture0, V.Texcoord).r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
