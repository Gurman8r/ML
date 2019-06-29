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
	float	fov;
	float	zNear;
	float	zFar;
} camera;

uniform struct Tex
{
	sampler2D diff_map;
	sampler2D spec_map;
} tex;

uniform struct Frag
{
	float	specular;
	int		shininess;
	vec3	lightPos;
	vec4	diffuse;
	float	ambient;
} frag;

uniform struct Time
{
	float	delta;
	float	total;
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
	float diff_amt = max(dot(diff_nml, diff_dir), 0.0);
	vec4  diff_col = vec4(diff_amt * frag.diffuse.rgb, 1.0);
	vec4  diff_tex = texture(tex.diff_map, V.Texcoord);
	vec4  diff_out = (diff_col * diff_tex);

	// Specular
	vec3  spec_nml = normalize(camera.position - V.Position);
	vec3  spec_dir = reflect(-diff_dir, diff_nml);
	float spec_amt = pow(max(dot(spec_nml, spec_dir), 0.0), frag.shininess);
	vec4  spec_col = vec4(frag.specular * spec_amt * frag.diffuse.rgb, 1.0);
	vec4  spec_tex = texture(tex.spec_map, V.Texcoord);
	vec4  spec_out = (spec_col * spec_tex);

	// Output
	gl_Color = V.Normal * (ambi_out + diff_out + spec_out);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */