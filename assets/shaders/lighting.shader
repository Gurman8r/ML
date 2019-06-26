// lighting.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.MVP.shader>
#shader vertex

void main()
{
	gl_Position = ml_MVP_Position();
	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.shader>
#shader fragment

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
	vec4	specular= (specCol * specTex);

	// Output
	gl_Color = Frag.mainCol * (ambient + diffuse + specular);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */