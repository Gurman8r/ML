// noobs_0.fs.shader
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

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Vert_Uniforms
{
	mat4		proj;
	mat4		view;
	mat4		model;
} Vert;

uniform struct Frag_Uniforms
{
	vec3		lightPos;
	vec4		diffuse;
	float		ambient;
	float		specular;
	int			shininess;
	vec4		mainCol;
	sampler2D	mainTex;
	sampler2D	specTex;
} Frag;

uniform struct Time_Uniforms
{
	float		delta;
	float		total;
} Time;

uniform struct Window_Uniforms
{
	vec2		size;
	vec4		color;
} Window;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Camera
	vec3 cameraPos = vec3(
		Vert.view[0][3],
		Vert.view[1][3],
		Vert.view[2][3]
	);
	
	// Ambient
	vec4  ambientOut  = (Frag.ambient * Frag.diffuse);

	// Diffuse
	vec3  diffNormal  = normalize(In.Normal.xyz);
	vec3  diffDir     = normalize(Frag.lightPos - In.Position);
	float diffAmount  = max(dot(diffNormal, diffDir), 0.0);
	vec4  diffColor   = vec4(diffAmount * Frag.diffuse.rgb, 1.0);
	vec4  diffTexture = texture(Frag.mainTex, In.Texcoord);
	vec4  diffuseOut  = (diffColor * diffTexture);

	// Specular
	vec3  specCamera  = normalize(cameraPos - In.Position);
	vec3  specReflect = reflect(-diffDir, diffNormal);
	float specAmount  = pow(max(dot(specCamera, specReflect), 0.0), Frag.shininess);
	vec4  specColor   = vec4(Frag.specular * specAmount * Frag.diffuse.rgb, 1.0);
	vec4  specTexture = texture(Frag.specTex, In.Texcoord);
	vec4  specularOut = (specColor * specTexture);

	// Output
	gl_Color = 
		(Frag.mainCol * In.Normal) *
		(ambientOut + diffuseOut + specularOut);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */