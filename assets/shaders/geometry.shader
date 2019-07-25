// geometry.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.shader"

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	V.Position	= a_Position;
	V.Normal		= a_Normal;
	V.Texcoord	= a_Texcoord;
	gl_Position		= (u_proj * u_view * u_model) * vec4(V.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Frag.shader"

uniform vec4 u_mainColor;
uniform sampler2D u_mainTexture;

void main()
{
	gl_Color = u_mainColor;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Curve.Draw.shader"

#define SAMPLES_PER_SEGMENT 16
#define SAMPLES_MAX 128
#define CURVE_MODE_LINES 0
#define CURVE_MODE_BEZIER 1
#define CURVE_MODE_CATMULLROM 2
#define CURVE_MODE_HERMITE 3

/* * * * * * * * * * * * * * * * * * * * */

layout(points) in;
layout(line_strip, max_vertices = SAMPLES_MAX) out;

/* * * * * * * * * * * * * * * * * * * * */

uniform int		u_mode;
uniform float	u_delta;
uniform float	u_size;
uniform int		u_samples;

/* * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */