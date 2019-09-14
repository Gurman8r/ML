#shader vertex
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

void main()
{
	V.Position = a_Position;
	V.Normal = a_Normal;
	V.Texcoord = a_Texcoord;
	gl_Position = vec4(V.Position.x, V.Position.y, 0.0, 1.0);
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
	gl_Color = u_color * texture(u_texture0, V.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */