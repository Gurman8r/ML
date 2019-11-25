// text.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

mat4 orthographic(float left, float right, float bottom, float top)
{
    mat4 temp = mat4(1.0);
    temp[0][0] = 2.0 / (right - left);
    temp[1][1] = 2.0 / (top - bottom);
    temp[2][2] = -1;
    temp[3][0] = -(right + left) / (right - left);
    temp[3][1] = -(top + bottom) / (top - bottom);
    return temp;
}

uniform vec2 u_resolution;

void main()
{
    V.position = a_position;
    V.normal = a_normal;
    V.texcoord = a_texcoord;

    mat4 p = orthographic(
        0.0, u_resolution.x,
        0.0, u_resolution.y
    );

    gl_Position = p * vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4 u_color;
uniform sampler2D u_texture0;

void main()
{
    gl_Color = u_color * vec4(1, 1, 1, texture(u_texture0, V.texcoord).r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
