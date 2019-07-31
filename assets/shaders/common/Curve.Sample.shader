vec4 lerp(in vec4 a, in vec4 b, const float t)
{
	return (b * t) + (a * (1.0 - t));
}

vec4 sampleBezier0(in vec4 p0, const float t)
{
	return p0;
}

vec4 sampleBezier1(in vec4 p0, in vec4 p1, const float t)
{
	return lerp(p0, p1, t);
}

vec4 sampleBezier2(in vec4 p0, in vec4 p1, in vec4 p2, const float t)
{
	return lerp(lerp(p0, p1, t), lerp(p1, p2, t), t);
}

vec4 sampleBezier3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const float t)
{
	return lerp(sampleBezier2(p0, p1, p2, t), sampleBezier2(p1, p2, p3, t), t);
}

vec4 sampleCatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float t)
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

vec4 sampleCubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float t)
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