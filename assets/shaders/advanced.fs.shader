// demo.fs.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

in Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform struct Camera
{
	vec3	pos;		// Position of camera
	float	fov;		// Field of View
	float	near;		// Near Clipping Distance
	float	far;		// Far Clipping Distance
} u_camera;

uniform vec2		u_cursorPos;	// Position of Cursor
uniform float		u_deltaTime;	// Elapsed Frame Time
uniform int			u_frameCount;	// Current Frame Vdex
uniform vec2		u_viewport;		// Size of Main Window
uniform float		u_totalTime;	// Total Time Elapsed (seconds)
uniform vec4		u_color;		// Main Color
uniform vec3		u_lightPos;		// Position of Light
uniform vec4		u_diffuse;		// Diffuse
uniform vec4		u_specular;		// Specular
uniform vec4		u_ambient;		// Ambient
uniform float		u_shininess;	// Shininess
uniform sampler2D	u_texture0;		// Diffuse Map
uniform sampler2D	u_texture1;		// Specular Map

void main()
{
	// Ambient
	vec4 ambient = (u_ambient * u_diffuse);

	// Diffuse
	vec3  diff_nml = normalize(V.normal.xyz);
	vec3  diff_dir = normalize(u_lightPos - V.position);
	float diff_amt = max(dot(diff_nml, diff_dir), 0.0);
	vec4  diff_col = vec4(diff_amt * u_diffuse.rgb, 1.0);
	vec4  diffuse  = texture(u_texture0, V.texcoord) * diff_col;

	// Specular
	vec3  spec_nml = normalize(u_camera.pos - V.position);
	vec3  spec_dir = reflect(-diff_dir, diff_nml);
	float spec_amt = pow(max(dot(spec_nml, spec_dir), 0.0), u_shininess);
	vec4  spec_col = vec4(spec_amt * u_specular.rgb, 1.0);
	vec4  specular = texture(u_texture1, V.texcoord) * spec_col;

	// Output
	gl_Color = V.normal * u_color * (ambient + diffuse + specular);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */