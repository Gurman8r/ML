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

uniform float   u_time;     // Total Time
uniform vec3    u_position; // Model Position
uniform vec3    u_scale;    // Model Scale
uniform vec4    u_rotation; // Model Rotation

mat4 transform(vec3 pos, vec4 rot)
{
    vec3  v = normalize(rot.xyz);
    float s = sin(rot.w);
    float c = cos(rot.w);
    float o = (1. - c);
    return mat4(
        (o * v.x * v.x + c), (o * v.x * v.y - v.z * s), (o * v.z * v.x + v.y * s), 0.,
        (o * v.x * v.y + v.z * s), (o * v.y * v.y + c), (o * v.y * v.z - v.x * s), 0.,
        (o * v.z * v.x - v.y * s), (o * v.y * v.z + v.x * s), (o * v.z * v.z + c), 0.,
        pos.x, pos.y, pos.z, 1.
    );
}

mat4 look_at(vec3 pos, vec3 dir)
{
    vec3 up = vec3(0, 1, 0);
    vec3 center = pos + normalize(dir);
    vec3 f = normalize(center - pos);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    mat4 m = mat4(1.);
    m[0][0] = s.x; m[1][0] = s.y; m[2][0] = s.z;
    m[0][1] = u.x; m[1][1] = u.y;  m[2][1] = u.z;
    m[0][2] = -f.x; m[1][2] = -f.y; m[2][2] = -f.z;
    m[3][0] = -dot(s, pos); m[3][1] = -dot(u, pos); m[3][2] = dot(f, pos);
    return m;
}

mat4 perspective(float fov, float aspect, float near, float far)
{
    return mat4(
        1. / (aspect * tan(fov / 2.)), 0., 0., 0.,
        0., 1. / tan(fov / 2.), 0., 0.,
        0., 0., -(far + near) / (far - near), -1.,
        0., 0., -(2. * far * near) / (far - near), 0.
    );
}

void main()
{
    // Model Matrix
    mat4 m = transform(u_position, vec4(u_rotation.xyz, u_rotation.w * u_time));

    // View Matrix
    mat4 v = look_at(u_camera.pos, u_camera.dir);

    // Projection Matrix
    mat4 p = perspective(
        u_camera.fov, (u_camera.view.x / u_camera.view.y), u_camera.near, u_camera.far
    );

    // Output
    gl_Position = (p * v * m) * vec4(a_position * u_scale, 1.0);
    V.position  = gl_Position.xyz;
    V.normal    = (transpose(inverse(m)) * vec4(a_normal, 1.0)).xyz;
    V.texcoord  = a_texcoord;
}