#shader vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

void main()
{
    V.position  = a_position;
    V.normal    = a_normal;
    V.texcoord  = a_texcoord;
    gl_Position = vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4        u_color;
uniform sampler2D   u_texture0;
uniform int         u_effect;
uniform mat3        u_kernel;

#define MODE_DEFAULT   0
#define MODE_GRAYSCALE 1
#define MODE_BLUR      2
#define MODE_KERNEL    3
#define MODE_INVERTED  4

vec4 drawKernel(in mat3 kernel)
{
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top    left
        vec2(0.0f,    offset),  // top    center
        vec2(offset,  offset),  // top    right
        vec2(-offset, 0.0f),    // center left
        vec2(0.0f,    0.0f),    // center center
        vec2(offset,  0.0f),    // center right
        vec2(-offset, -offset), // bottom left
        vec2(0.0f,    -offset), // bottom center
        vec2(offset,  -offset)  // bottom right    
    );

    vec3 samples[9];
    for (int i = 0; i < 9; i++)
    {
        samples[i] = vec3(texture(u_texture0, V.texcoord.st + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        color += samples[i] * kernel[i / 3][i % 3];
    }

    return vec4(color, 1.0);
}

void main()
{
    switch (u_effect)
    {
    case MODE_GRAYSCALE:
        gl_Color = texture(u_texture0, V.texcoord);
        float average = (gl_Color.r + gl_Color.g + gl_Color.b) / 3.0;
        gl_Color = vec4(average, average, average, 1.0);
        break;

    case MODE_KERNEL:
        gl_Color = drawKernel(u_kernel);
        break;

    case MODE_BLUR:
        gl_Color = drawKernel(mat3(
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        ));
        break;

    case MODE_INVERTED:
        gl_Color = vec4(vec3(1.0 - texture(u_texture0, V.texcoord)), 1.0);
        break;

    case MODE_DEFAULT:
    default:
        gl_Color = texture(u_texture0, V.texcoord);
        break;
    };
}
