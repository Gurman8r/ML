// geometry.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.MVP.shader>
#shader vertex

void main()
{
	gl_Position = ml_MVP_Position();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.Draw.shader>
#shader fragment

void main()
{
	gl_Color = Frag.mainCol;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Geom.Curve.shader>
#shader geometry

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

struct Geom_Uniforms
{
	int		mode;
	float_t	delta;
	float_t	size;
	int		samples;
};
uniform Geom_Uniforms Geom;

/* * * * * * * * * * * * * * * * * * * * */

void stub(in int samples, in float_t dt)
{
	// test points
	float_t size = Geom.size;
	vec4 testP0 = vec4(-size, +size, 0.0, 1.0);
	vec4 testP1 = vec4(+size, +size, 0.0, 1.0);
	vec4 testP2 = vec4(+size, -size, 0.0, 1.0);
	vec4 testP3 = vec4(-size, -size, 0.0, 1.0);

	vec4 p0, p1, p2, p3, pPrev, pNext, m0, m1;

	switch (Geom.mode)
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
	stub(Geom.samples, (Geom.delta / float_t(Geom.samples)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */