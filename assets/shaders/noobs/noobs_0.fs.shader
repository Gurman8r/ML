// noobs_0.fs.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

out vec4 gl_Color;

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Camera
{
	vec3	position;
	vec3	target;
	float_t	fov;
	float_t	zNear;
	float_t	zFar;
} camera;

uniform struct Frag
{
	sampler2D tex_dm;
	sampler2D tex_sm;

	float_t	specular;
	int		shininess;
	vec3	lightPos;
	vec4	diffuse;
	float_t	ambient;
} frag;

uniform struct Time
{
	float_t	delta;
	float_t	total;
} time;

uniform struct Window
{
	vec2	size;
	vec4	color;
} window;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4  ambi_out = (frag.ambient * frag.diffuse);

	// Diffuse
	vec3  diff_nml = normalize(V.Normal.xyz);
	vec3  diff_dir = normalize(frag.lightPos - V.Position);
	float_t diff_amt = max(dot(diff_nml, diff_dir), 0.0);
	vec4  diff_col = vec4(diff_amt * frag.diffuse.rgb, 1.0);
	vec4  diff_tex = texture(frag.tex_dm, V.Texcoord);
	vec4  diff_out = (diff_col * diff_tex);

	// Specular
	vec3  spec_nml = normalize(camera.position - V.Position);
	vec3  spec_dir = reflect(-diff_dir, diff_nml);
	float_t spec_amt = pow(max(dot(spec_nml, spec_dir), 0.0), frag.shininess);
	vec4  spec_col = vec4(frag.specular * spec_amt * frag.diffuse.rgb, 1.0);
	vec4  spec_tex = texture(frag.tex_sm, V.Texcoord);
	vec4  spec_out = (spec_col * spec_tex);

	// Output
	gl_Color = V.Normal * (ambi_out + diff_out + spec_out);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */