#shader vertex
#include "../../../assets/shaders/common/Vert.shader"

mat4 look_at(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);
	mat4 m = mat4(1.0);
	m[0][0] = s.x; m[1][0] = s.y; m[2][0] = s.z;
	m[0][1] = u.x; m[1][1] = u.y; m[2][1] = u.z;
	m[0][2] = -f.x; m[1][2] = -f.y; m[2][2] = -f.z;
	m[3][0] = -dot(s, eye); m[3][1] = -dot(u, eye); m[3][2] = dot(f, eye);
	return m;
}

mat4 perspective(float fov, float aspect, float near, float far)
{
	mat4 m;
	m[0][0] = 1.0 / (aspect * tan(fov / 2.0));
	m[1][1] = 1.0 / tan(fov / 2.0);
	m[2][3] = -1.0;
	m[2][2] = -(far + near) / (far - near);
	m[3][2] = -(2.0 * far * near) / (far - near);
	return m;
}

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Camera
{
	vec3	pos;	// Position of camera
	float	fov;	// Field of View
	float	near;	// Near Clipping Distance
	float	far;	// Far Clipping Distance
} u_camera;

uniform vec2	u_cursorPos;	// Position of Cursor
uniform float	u_deltaTime;	// Elapsed Frame Time
uniform int		u_frameCount;	// Current Frame Index
uniform vec2	u_viewport;		// Size of Main Window
uniform float	u_totalTime;	// Total Time Elapsed (seconds)

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Attributes
	V.Position	= a_Position;
	V.Normal	= a_Normal;
	V.Texcoord	= a_Texcoord;

	// Model Matrix
	mat4 m = mat4(1);
	m[3] = vec4(0, 0, 0, 1);

	// View Matrix
	mat4 v = look_at(u_camera.pos, m[3].xyz, vec3(0.0, 1.0, 0.0));

	// Projection Matrix
	mat4 p = perspective(
		u_camera.fov, (u_viewport.x / u_viewport.y), u_camera.near, u_camera.far
	);

	// MVP
	gl_Position	= (p * v * m) * vec4(V.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
#include "../../../assets/shaders/common/Frag.shader"

uniform	vec4		u_color;
uniform sampler2D	u_texture0;

void main()
{
	gl_Color = u_color * texture(u_texture0, V.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
