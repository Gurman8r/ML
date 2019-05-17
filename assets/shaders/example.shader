#shader vertex
#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;
out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;
void ml_Update_Vertex()
{
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;
}
struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};
uniform Vert_Uniforms Vert;
mat4 ml_MVP()
{
	return (Vert.proj * Vert.view * Vert.model);
}
vec4 ml_MVP_Position()
{
	ml_Update_Vertex();

	return ml_MVP() * vec4(Out.Position, 1.0);
}
void main()
{
	gl_Position = ml_MVP_Position();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#version 410 core
in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;
out vec4 gl_Color;
struct Frag_Uniforms
{
	vec4 mainCol;
	sampler2D mainTex;
};
uniform Frag_Uniforms Frag;
void main()
{
	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */