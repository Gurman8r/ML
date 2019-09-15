#include "../../../assets/shaders/common/Curve.Sample.shader"

void drawLine(in vec4 a, in vec4 b, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
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

void drawBezierCurve0(in vec4 a, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier0(a, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve1(in vec4 a, in vec4 b, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier1(a, b, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve2(in vec4 a, in vec4 b, in vec4 c, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier2(a, b, c, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawBezierCurve3(in vec4 a, in vec4 b, in vec4 c, in vec4 d, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier3(a, b, c, d, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawCatmullRomSplineSegment(in vec4 pPrev, in vec4 a, in vec4 b, in vec4 pNext, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCatmullRom(pPrev, a, b, pNext, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

void drawCubicHermiteSplineSegment(in vec4 a, in vec4 m0, in vec4 b, in vec4 m1, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCubicHermite(a, m0, b, m1, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}