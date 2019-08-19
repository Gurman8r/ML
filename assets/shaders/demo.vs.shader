// demo.vs.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out Vertex {
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

/* * * * * * * * * * * * * * * * * * * * */

mat4 angle_axis(vec3 v, float angle)
{
	v = normalize(v);
	float s = sin(angle);
	float c = cos(angle);
	float o = 1.0 - c;
	return mat4(
		o * v.x * v.x + c, o * v.x * v.y - v.z * s, o * v.z * v.x + v.y * s, 0.0,
		o * v.x * v.y + v.z * s, o * v.y * v.y + c, o * v.y * v.z - v.x * s, 0.0,
		o * v.z * v.x - v.y * s, o * v.y * v.z + v.x * s, o * v.z * v.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 look_at(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);
	mat4 m = mat4(1.0);
	m[0][0] = s.x;
	m[1][0] = s.y;
	m[2][0] = s.z;
	m[0][1] = u.x;
	m[1][1] = u.y;
	m[2][1] = u.z;
	m[0][2] = -f.x;
	m[1][2] = -f.y;
	m[2][2] = -f.z;
	m[3][0] = -dot(s, eye);
	m[3][1] = -dot(u, eye);
	m[3][2] = dot(f, eye);
	return m;
}

mat4 perspective(float fov, float aspect, float zNear, float zFar)
{
	mat4 m;
	m[0][0] = 1.0 / (aspect * tan(fov / 2.0));
	m[1][1] = 1.0 / tan(fov / 2.0);
	m[2][3] = -1.0;
	m[2][2] = -(zFar + zNear) / (zFar - zNear);
	m[3][2] = -(2.0 * zFar * zNear) / (zFar - zNear);
	return m;
}

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Camera
{
	vec3		pos;		// Position of camera
	vec3		target;		// Where is the camera looking?
	float		fov;		// Field of View
	float		zNear;		// Near Clipping Distance
	float		zFar;		// Far Clipping Distance
} u_camera;

uniform vec2	u_cursorPos;	// Position of Cursor
uniform float	u_deltaTime;	// Elapsed Frame Time
uniform int		u_frameCount;	// Current Frame Index
uniform vec2	u_viewport;		// Size of Main Window
uniform float	u_totalTime;	// Total Time Elapsed (seconds)

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Model Matrix
	mat4 model = angle_axis(vec3(0.0, 1.0, 0.0), u_totalTime * 0.33);

	// View Matrix
	mat4 view = look_at(
		u_camera.pos, u_camera.target, vec3(0.0, 1.0, 0.0)
	);

	// Projection Matrix
	mat4 proj = perspective(
		u_camera.fov, (u_viewport.x / u_viewport.y), u_camera.zNear, u_camera.zFar
	);

	// Output
	gl_Position = (proj * view * model) * vec4(a_Position, 1.0);
	V.Position	= gl_Position.xyz;
	V.Normal	= transpose(inverse(model)) * a_Normal;
	V.Texcoord	= a_Texcoord;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
