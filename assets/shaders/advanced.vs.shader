// advanced.vs.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec4 normal; vec2 texcoord; } V;

mat4 transform(vec3 pos, vec4 rot)
{
	vec3  v = normalize(rot.xyz);
	float s = sin(rot.w);
	float c = cos(rot.w);
	float o = (1.0 - c);
	return mat4(
		(o * v.x * v.x + c), (o * v.x * v.y - v.z * s), (o * v.z * v.x + v.y * s), 0.0,
		(o * v.x * v.y + v.z * s), (o * v.y * v.y + c), (o * v.y * v.z - v.x * s), 0.0,
		(o * v.z * v.x - v.y * s), (o * v.y * v.z + v.x * s), (o * v.z * v.z + c), 0.0,
		pos.x, pos.y, pos.z, 1.0
	);
}

mat4 look_at(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);
	mat4 m = mat4(1.0);
	m[0][0] = s.x; m[1][0] = s.y; m[2][0] = s.z;
	m[0][1] = u.x; m[1][1] = u.y;  m[2][1] = u.z;
	m[0][2] = -f.x; m[1][2] = -f.y; m[2][2] = -f.z;
	m[3][0] = -dot(s, eye); m[3][1] = -dot(u, eye); m[3][2] = dot(f, eye);
	return m;
}

mat4 perspective(float fov, float aspect, float near, float far)
{
	return mat4(
		1.0 / (aspect * tan(fov / 2.0)), 0.0, 0.0, 0.0,
		0.0, 1.0 / tan(fov / 2.0), 0.0, 0.0,
		0.0, 0.0, -(far + near) / (far - near), -1.0,
		0.0, 0.0, -(2.0 * far * near) / (far - near), 0.0
	);
}

/* * * * * * * * * * * * * * * * * * * * */

uniform struct Camera
{
	vec3	pos;	// Camera Position
	float	fov;	// Field of View
	float	near;	// Near Clipping Distance
	float	far;	// Far Clipping Distance
} u_camera;

uniform vec2	u_cursor;	// Cursor Position
uniform float	u_delta;	// Delta Time
uniform int		u_frame;	// Frame Index
uniform float	u_fps;		// Frame Rate
uniform vec2	u_viewport;	// Frame Size
uniform float	u_time;		// Total Time
uniform vec3	u_position;	// Model Position
uniform vec3	u_scale;	// Model Scale

void main()
{
	// Model Matrix
	mat4 m = transform(u_position, vec4(0.0, 1.0, 0.0, u_time * 0.33));

	// View Matrix
	mat4 v = look_at(u_camera.pos, vec3(0), vec3(0.0, 1.0, 0.0));

	// Projection Matrix
	mat4 p = perspective(
		u_camera.fov, (u_viewport.x / u_viewport.y), u_camera.near, u_camera.far
	);

	// Output
	gl_Position = (p * v * m) * vec4(a_position * u_scale, 1.0);
	V.position	= gl_Position.xyz;
	V.normal	= transpose(inverse(m)) * a_normal;
	V.texcoord	= a_texcoord;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */