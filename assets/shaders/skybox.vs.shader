#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

uniform struct Camera
{
    vec3    pos;    // Camera Position
    vec3    dir;    // Camera Direction
    float   fov;    // Field of View
    float   near;   // Near Clipping Distance
    float   far;    // Far Clipping Distance
    vec2    view;   // Display Size
} u_camera;

uniform float u_time;
uniform vec4 u_rotation;

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

void main()
{
    // Model Matrix
    mat4 m = angle_axis(u_rotation.xyz, u_rotation.w * u_time);
    m[3] = vec4(u_camera.pos, 1);

    // View Matrix
    mat4 v = look_at(u_camera.pos - vec3(0, 0, 0.1), m[3].xyz, vec3(0.0, 1.0, 0.0));

    // Projection Matrix
    mat4 p = perspective(
        u_camera.fov, (u_camera.view.x / u_camera.view.y), u_camera.near, u_camera.far
    );

    // Output
    V.position = a_position;
    V.normal = a_normal;
    V.texcoord = a_texcoord;
    gl_Position = (p * v * m) * vec4(a_position, 1.0);
}