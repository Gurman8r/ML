// noobs.fs.shader
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
	vec3	position;	// Position of camera
	vec3	target;		// Where is the camera looking?
	float	fov;		// Field of View
	float	zNear;		// Near Clipping Distance
	float	zFar;		// Far Clipping Distance
} camera;

uniform vec2		u_cursorPos;	// Position of Cursor
uniform float		u_deltaTime;	// Elapsed Frame Time
uniform int			u_frameCount;	// Current Frame Index
uniform vec2		u_resolution;	// Size of Main Window
uniform float		u_totalTime;	// Total Time Elapsed (seconds)

uniform sampler2D	u_diff_tex;		// Diffuse Map
uniform sampler2D	u_spec_tex;		// Specular Map
uniform vec3		u_lightPos;		// Position of Light
uniform float		u_specular;		// Specular
uniform int			u_shininess;	// Shininess
uniform vec4		u_diffuse;		// Diffuse
uniform float		u_ambient;		// Ambient


/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4  ambi_out = (u_ambient * u_diffuse);

	// Diffuse
	vec3  diff_nml = normalize(V.Normal.xyz);
	vec3  diff_dir = normalize(u_lightPos - V.Position);
	float diff_amt = max(dot(diff_nml, diff_dir), 0.0);
	vec4  diff_col = vec4(diff_amt * u_diffuse.rgb, 1.0);
	vec4  diff_tex = texture(u_diff_tex, V.Texcoord);
	vec4  diff_out = (diff_col * diff_tex);

	// Specular
	vec3  spec_nml = normalize(camera.position - V.Position);
	vec3  spec_dir = reflect(-diff_dir, diff_nml);
	float spec_amt = pow(max(dot(spec_nml, spec_dir), 0.0), u_shininess);
	vec4  spec_col = vec4(u_specular * spec_amt * u_diffuse.rgb, 1.0);
	vec4  spec_tex = texture(u_spec_tex, V.Texcoord);
	vec4  spec_out = (spec_col * spec_tex);

	// Output
	gl_Color = V.Normal * (ambi_out + diff_out + spec_out);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */