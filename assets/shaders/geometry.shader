#shader vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

mat4 ortho(float left, float right, float bottom, float top)
{
    mat4 temp = mat4(1.0);
    temp[0][0] = 2.0 / (right - left);
    temp[1][1] = 2.0 / (top - bottom);
    temp[2][2] = -1;
    temp[3][0] = -(right + left) / (right - left);
    temp[3][1] = -(top + bottom) / (top - bottom);
    return temp;
}

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    V.position = a_position;
    V.normal = a_normal;
    V.texcoord = a_texcoord;

    gl_Position = (u_proj * u_view * u_model) * vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4 u_color;

void main()
{
    gl_Color = u_color;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader geometry
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

#define SAMPLES_PER_SEGMENT 16
#define SAMPLES_MAX 128
#define CURVE_MODE_LINES 0
#define CURVE_MODE_BEZIER 1
#define CURVE_MODE_CATMULLROM 2
#define CURVE_MODE_HERMITE 3

layout(points) in;
layout(line_strip, max_vertices = SAMPLES_MAX) out;

#include "../../../assets/shaders/common/Curve.Draw.shader"

uniform int     u_mode;
uniform float   u_delta;
uniform float   u_size;
uniform int     u_samples;

void stub(in int samples, in float dt)
{
    // test points
    float size = u_size;
    vec4 testP0 = vec4(-size, +size, 0.0, 1.0);
    vec4 testP1 = vec4(+size, +size, 0.0, 1.0);
    vec4 testP2 = vec4(+size, -size, 0.0, 1.0);
    vec4 testP3 = vec4(-size, -size, 0.0, 1.0);

    vec4 p0, p1, p2, p3, pPrev, pNext, m0, m1;

    switch (u_mode)
    {
    case CURVE_MODE_LINES:
        // multiple segments
        drawLine(testP0, testP1, samples, dt);
        drawLine(testP1, testP2, samples, dt);
        drawLine(testP2, testP3, samples, dt);
        drawLine(testP3, testP0, samples, dt);
        break;

    case CURVE_MODE_BEZIER:
        // full curve
        drawBezierCurve3(testP0, testP1, testP2, testP3, samples, dt);
        break;

    case CURVE_MODE_CATMULLROM:
        // curve segment
        pPrev = testP0;
        p0 = testP1;
        p1 = testP2;
        pNext = testP3;
        drawCatmullRomSplineSegment(pPrev, p0, p1, pNext, samples, dt);
        break;

    case CURVE_MODE_HERMITE:
        // curve segment
        p0 = testP0;
        m0 = testP1 - p0;
        p1 = testP2;
        m1 = testP3 - p1;
        drawCubicHermiteSplineSegment(p0, m0, p1, m1, samples, dt);
        // tangents
        drawLineFull(p0, p0 + m0);
        drawLineFull(p1, p1 + m1);
        break;

    default:
        drawLineFull(testP0, testP1);
        drawLineFull(testP1, testP2);
        drawLineFull(testP2, testP3);
        drawLineFull(testP3, testP0);
        break;
    };
}

void main()
{
    stub(u_samples, (u_delta / float(u_samples)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */