// Geom.Curve.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader geometry
#version 410 core

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

vec4 lerp(in vec4 a, in vec4 b, const float_t t)
{
	return (b * t) + (a * (1.0 - t));
}

vec4 sampleBezier0(in vec4 p0, const float_t t)
{
	return p0;
}
vec4 sampleBezier1(in vec4 p0, in vec4 p1, const float_t t)
{
	return lerp(p0, p1, t);
}
vec4 sampleBezier2(in vec4 p0, in vec4 p1, in vec4 p2, const float_t t)
{
	return lerp(lerp(p0, p1, t), lerp(p1, p2, t), t);
}
vec4 sampleBezier3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const float_t t)
{
	return lerp(sampleBezier2(p0, p1, p2, t), sampleBezier2(p1, p2, p3, t), t);
}

vec4 sampleCatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float_t t)
{
	mat4 a = mat4(pPrev, p0, p1, pNext);

	mat4 b = mat4(
		0.0, -1.0, 2.0, -1.0,
		2.0, 0.0, -5.0, 3.0,
		0.0, 1.0, 4.0, -3.0,
		0.0, 0.0, -1.0, 1.0) * 0.5f;
	b = transpose(b);

	vec4 c = vec4(1.0, t, t * t, t * t * t);

	return a * b * c;
}

vec4 sampleCubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float_t t)
{
	mat4 a = mat4(p0, m0, p1, m1);

	mat4 b = mat4(
		1.0, 0.0, -3.0, 2.0,
		0.0, 1.0, -2.0, 1.0,
		0.0, 0.0, 3.0, -2.0,
		0.0, 0.0, -1.0, 1.0);
	b = transpose(b);

	vec4 c = vec4(1.0, t, t * t, t * t * t);

	return a * b * c;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void drawLine(in vec4 a, in vec4 b, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = lerp(a, b, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawLineFull(in vec4 a, in vec4 b)
{
	gl_Position = a;
	EmitVertex();
	gl_Position = b;
	EmitVertex();
	EndPrimitive();
}

void drawBezierCurve0(in vec4 a, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier0(a, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve1(in vec4 a, in vec4 b, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier1(a, b, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve2(in vec4 a, in vec4 b, in vec4 c, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier2(a, b, c, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve3(in vec4 a, in vec4 b, in vec4 c, in vec4 d, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier3(a, b, c, d, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawCatmullRomSplineSegment(in vec4 pPrev, in vec4 a, in vec4 b, in vec4 pNext, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCatmullRom(pPrev, a, b, pNext, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawCubicHermiteSplineSegment(in vec4 a, in vec4 m0, in vec4 b, in vec4 m1, const int samples, const float_t dt)
{
	int i = 0;
	float_t t = 0.0;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCubicHermite(a, m0, b, m1, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */