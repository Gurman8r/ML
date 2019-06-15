// Vertex
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

struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

uniform Vert_Uniforms Vert;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;

	gl_Position = (Vert.proj * Vert.view * Vert.model) * vec4(Out.Position, 1.0);

	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}


// Fragment
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

out vec4 gl_Color;

struct Frag_Uniforms
{
	vec3		cameraPos;
	vec3		lightPos;
	vec4		diffuse;
	float		ambient;
	float		specular;
	int			shininess;
	vec4		mainCol;
	sampler2D	mainTex;
	sampler2D	specTex;
};

uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4	ambient = (Frag.ambient * Frag.diffuse);

	// Diffuse 
	vec3	diffNml = normalize(In.Normal.xyz);
	vec3	diffDir = normalize(Frag.lightPos - In.Position);
	float	diffAmt = max(dot(diffNml, diffDir), 0.0);
	vec4	diffCol = vec4(diffAmt * Frag.diffuse.rgb, 1.0);
	vec4	diffTex = texture(Frag.mainTex, In.Texcoord);
	vec4	diffuse = (diffCol * diffTex);

	// Specular		 
	vec3	specCam = normalize(Frag.cameraPos - In.Position);
	vec3	specRfl = reflect(-diffDir, diffNml);
	float	specAmt = pow(max(dot(specCam, specRfl), 0.0), Frag.shininess);
	vec4	specCol = vec4(Frag.specular * specAmt * Frag.diffuse.rgb, 1.0);
	vec4	specTex = texture(Frag.specTex, In.Texcoord);
	vec4	specular = (specCol * specTex);

	// Output
	gl_Color = Frag.mainCol * (ambient + diffuse + specular);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */