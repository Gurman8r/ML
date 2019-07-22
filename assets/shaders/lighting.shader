// lighting.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.shader"

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	Out.Position	= a_Position;
	Out.Normal		= a_Normal;
	Out.Texcoord	= a_Texcoord;
	gl_Position		= (u_proj * u_view * u_model) * vec4(Out.Position, 1.0);
	Out.Position	= gl_Position.xyz;
	Out.Normal		= transpose(inverse(u_model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Frag.shader"

uniform vec4		u_mainColor;
uniform sampler2D	u_mainTexture;
uniform vec3		u_cameraPos;
uniform vec3		u_lightPos;
uniform vec4		u_diffuse;
uniform float		u_ambient;
uniform float		u_specular;
uniform int			u_shininess;
uniform sampler2D	u_specTex;

void main()
{
	// Ambient
	vec4	ambient = (u_ambient * u_diffuse);

	// Diffuse 
	vec3	diffNml = normalize(In.Normal.xyz);
	vec3	diffDir = normalize(u_lightPos - In.Position);
	float	diffAmt = max(dot(diffNml, diffDir), 0.0);
	vec4	diffCol = vec4(diffAmt * u_diffuse.rgb, 1.0);
	vec4	diffTex = texture(u_mainTexture, In.Texcoord);
	vec4	diffuse = (diffCol * diffTex);

	// Specular		 
	vec3	specCam = normalize(u_cameraPos - In.Position);
	vec3	specRfl = reflect(-diffDir, diffNml);
	float	specAmt = pow(max(dot(specCam, specRfl), 0.0), u_shininess);
	vec4	specCol = vec4(u_specular * specAmt * u_diffuse.rgb, 1.0);
	vec4	specTex = texture(u_specTex, In.Texcoord);
	vec4	specular= (specCol * specTex);

	// Output
	gl_Color = u_mainColor * (ambient + diffuse + specular);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */