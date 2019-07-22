// noobs_0.vs.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 410 core

/* * * * * * * * * * * * * * * * * * * * */

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} V;

/* * * * * * * * * * * * * * * * * * * * */

mat4 ml_AngleAxis(vec3 v, float angle)
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

mat4 ml_LookAt(vec3 eye, vec3 center, vec3 up)
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

mat4 ml_Perspective(float fov, float aspect, float zNear, float zFar)
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
	vec3		position;	// Position of camera
	vec3		target;		// Where is the camera looking?
	float		fov;		// Field of View
	float		zNear;		// Near Clipping Distance
	float		zFar;		// Far Clipping Distance
} camera;

uniform struct System
{
	vec2		cursorPos;	// Position of Cursor
	float		deltaTime;	// Elapsed Frame Time
	int			frameCount;	// Current Frame Index
	vec2		resolution;	// Size of Main Window
	float		totalTime;	// Total Time Elapsed (seconds)
} sys;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Position
	mat4 pos = mat4(0.0);
	pos[3][0] = 0.0; // x
	pos[3][1] = sin(sys.totalTime); // y
	pos[3][2] = 0.0; // z

	// Rotation
	mat4 rot = ml_AngleAxis(vec3(0.0, 1.0, 0.0), sys.totalTime);
	
	// Model Matrix
	mat4 model = pos + rot;

	// View Matrix
	mat4 view = ml_LookAt(
		camera.position,
		camera.target,
		vec3(0.0, 1.0, 0.0)
	);

	// Projection Matrix
	mat4 proj = ml_Perspective(
		camera.fov,
		(sys.resolution.x / sys.resolution.y),
		camera.zNear,
		camera.zFar
	);

	// Output
	gl_Position = (proj * view * model) * vec4(a_Position, 1.0);
	V.Position	= gl_Position.xyz;
	V.Normal	= transpose(inverse(model)) * a_Normal;
	V.Texcoord	= a_Texcoord;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */