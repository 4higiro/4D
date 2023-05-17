#version 130

uniform vec2 resolution;
uniform vec3 position;
uniform mat4 view;
uniform mat4 transform;
uniform float w;

float plane(vec3 ro, vec3 rd, vec3 i, vec3 j, vec3 k) 
{
	float A = (j.y - i.y) * (k.z - i.z) - (j.z - i.z) * (k.y - i.y);
	float B = (j.z - i.z) * (k.x - i.x) - (j.x - i.x) * (k.z - i.z);
	float C = (j.x - i.x) * (k.y - i.y) - (j.y - i.y) * (k.x - i.x);
	float D = -1.0f * (B * i.y + A * i.x + C * i.z);
	float t1 = -1.0f * (D + A * ro.x + B * ro.y + C * ro.z);
	float t2 = A * rd.x + B * rd.y + C * rd.z;
	if (t2 == 0.0f)
		return 0.0f;
	float t = t1 / t2;
	if (t >= 0.0f)
		return t;
	else
		return 0.0f;
}

vec4 hyperPlane(vec4 ro4, vec4 rd4, vec4 i, vec4 j, vec4 k, vec4 m)
{
	vec4 ap = j - i;
	vec4 bt = k - i;
	vec4 gm = m - i;
	float a = ap.y*(bt.z*gm.w - gm.z*bt.w) - ap.z*(bt.y*gm.w - gm.y*bt.w) + ap.w*(bt.y*gm.z - gm.y*bt.z);
	float b = -1.0f*(ap.x*(bt.z*gm.w - gm.z*bt.w) - ap.z*(bt.x*gm.w - gm.x*bt.w) + ap.w*(bt.x*gm.z - gm.x*bt.z));
	float c = ap.x*(bt.y*gm.w - gm.y*bt.w) - ap.y*(bt.x*gm.w - gm.x*bt.w) + ap.w*(bt.x*gm.y - gm.x*bt.y);
	float d = -1.0f*(ap.x*(bt.y*gm.z - gm.y*bt.z) - ap.y*(bt.x*gm.z - gm.x*bt.z) + ap.z*(bt.x*gm.y - gm.x*bt.y));
	float e = -1.0f*(a*j.x + b*j.y + c*j.z + d*j.w);
	float t1 = a*ro4.x + b*ro4.y + c*ro4.z + d*ro4.w + e;
	float t2 = a*rd4.x + b*rd4.y + c*rd4.z + d*rd4.w;
	if (t2 != 0.0f)
	{
		float t = -1.0f*t1/t2;
		if (t > 0.0f)
			return ro4 + rd4 * t;
		else return vec4(0.0f);
	}
	else if (t1 == 0.0f && t2 == 0.0f)
		return vec4(-1.0f);
	else 
		return vec4(0.0f);
}

vec4 elipHyperboloid(vec4 ro4, vec4 rd4, vec4 o)
{
	float c1 = rd4.x * rd4.x + rd4.y * rd4.y + rd4.z * rd4.z;
	float c2 = 2.0f * rd4.x * (ro4.x - o.x) + 2.0f * rd4.y * (ro4.y - o.y) + 2.0f * rd4.z * (ro4.z - o.z) - 2.0f * rd4.w;
	float c3 = ro4.x * ro4.x - 2.0f * ro4.x * o.x + ro4.y * ro4.y - 2.0f * ro4.y * o.y + ro4.z * ro4.z - 2.0f * ro4.z * o.z - 2.0f * ro4.w + o.x * o.x + o.y * o.y + o.z * o.z + 2.0f * o.w;
	float D = c2 * c2 - 4.0f * c1 * c3;
	if (D >= 0.0f)
	{
		float t1 = (-c2 - sqrt(D)) / (2.0f * c1);
		float t2 = (-c2 - sqrt(D)) / (2.0f * c1);
		if (t1 < 0.0f && t2 < 0.0f)
			return vec4(0.0f);
		else if (t1 > 0.0f && t2 > 0.0f)
		{
			float t = min(t1, t2);
			return ro4 + rd4 * t;
		}
		else if (t1 < 0.0f)
			return ro4 + rd4 * t2;
		else if (t2 < 0.0f)
			return ro4 + rd4 * t1;
	}
	else
		return vec4(0.0f);
}

float checkPoint(vec4 ro4, vec4 rd4, vec4 v1, vec4 v2, vec4 v3, vec4 i, float t1, float t)
{
	vec4 o = (ro4 + rd4 * t1) - i;
	float f1 = dot(v1, o) / length(v1);
	float f2 = dot(v2, o) / length(v2);
	float f3 = dot(v3, o) / length(v3);
	if (t1 > 0.0f && (t1 < t || t == 0.0f) && (f1 >= 0.0f && f1 <= length(v1) && f2 >= 0.0f && f2 <= length(v2) && f3 >= 0.0f && f3 <= length(v3)))
		return t1;
	return t;
}

vec4 limHyperPlane(vec4 ro4, vec4 rd4, vec4 i, vec4 j, vec4 k, vec4 m)
{	
	vec4 point = hyperPlane(ro4, rd4, i, j, k, m);
	vec4 v1 = j - i;
	vec4 v2 = k - i;
	vec4 v3 = m - i;
	if (point == vec4(-1.0f))
	{
		float t1 = plane(ro4.xyz, rd4.xyz, i.xyz, j.xyz, k.xyz);
		float t2 = plane(ro4.xyz, rd4.xyz, i.xyz, j.xyz, m.xyz);
		float t3 = plane(ro4.xyz, rd4.xyz, i.xyz, k.xyz, m.xyz);
		vec3 i1 = i.xyz + v1.xyz + v2.xyz + v3.xyz;
		vec3 j1 = i.xyz + v2.xyz + v3.xyz;
		vec3 k1 = i.xyz + v1.xyz + v3.xyz;
		vec3 m1 = i.xyz + v1.xyz + v2.xyz;
		float t4 = plane(ro4.xyz, rd4.xyz, i1, j1, k1);
		float t5 = plane(ro4.xyz, rd4.xyz, i1, j1, m1);
		float t6 = plane(ro4.xyz, rd4.xyz, i1, k1, m1);
		float t = checkPoint(ro4, rd4, v1, v2, v3, i, t1, 0.0f);
		t = checkPoint(ro4, rd4, v1, v2, v3, i, t2, t);
		t = checkPoint(ro4, rd4, v1, v2, v3, i, t3, t);
		t = checkPoint(ro4, rd4, v1, v2, v3, i, t4, t);
		t = checkPoint(ro4, rd4, v1, v2, v3, i, t5, t);
		t = checkPoint(ro4, rd4, v1, v2, v3, i, t6, t);
		return ro4 + rd4 * t;
	}
	else if (point != vec4(-1.0f) && point != vec4(0.0f))
	{
		vec4 v4 = point - i;
		float k1 = dot(v1, v4) / length(v1);
		float k2 = dot(v2, v4) / length(v2);
		float k3 = dot(v3, v4) / length(v3);
		if (k1 >= 0.0f && k1 <= length(v1) && k2 >= 0.0f && k2 <= length(v2) && k3 >= 0.0f && k3 <= length(v3))
			return point;
		return vec4(0.0f);
	}
	else
		return vec4(0.0f);
}

void main()
{
	vec4 pos = vec4(position, w);
	vec3 color = vec3(0.0f);

	vec2 uv = vec2(gl_TexCoord[0].xy - 0.5f) * resolution / resolution.y;
	vec3 ro = vec3(0.0f, 0.0f, 0.0f);
	vec3 rd = normalize(vec3(uv, 1.0f));

	vec4 ro4 = vec4(ro, 0.0f);
	vec4 rd4 = normalize(vec4(rd, 0.0f));

	vec4 light = view * (vec4(1.0f, 1.0f, 1.0f, 1.0f) - pos);

	vec4 i1 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 j1 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 k1 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 m1 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 n1 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos); 

	vec4 i2 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 j2 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 k2 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 1.0f) - pos);
	vec4 m2 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 1.0f) - pos);
	vec4 n2 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos);

	vec4 i3 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 j3 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 k3 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 m3 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 n3 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);

	vec4 i4 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 j4 = view * (transform * vec4(1.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 k4 = view * (transform * vec4(0.0f, 1.0f, 1.0f, 0.0f) - pos);
	vec4 m4 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 1.0f) - pos);
	vec4 n4 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);

	vec4 i5 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 j5 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 k5 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 m5 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 n5 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);

	vec4 i6 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 j6 = view * (transform * vec4(1.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 k6 = view * (transform * vec4(0.0f, 1.0f, 1.0f, 0.0f) - pos);
	vec4 m6 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 1.0f) - pos);
	vec4 n6 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);

	vec4 i7 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 j7 = view * (transform * vec4(0.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 k7 = view * (transform * vec4(0.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 m7 = view * (transform * vec4(0.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 n7 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);

	vec4 i8 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);
	vec4 j8 = view * (transform * vec4(1.0f, 1.0f, 0.0f, 0.0f) - pos);
	vec4 k8 = view * (transform * vec4(1.0f, 0.0f, 1.0f, 0.0f) - pos);
	vec4 m8 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 1.0f) - pos);
	vec4 n8 = view * (transform * vec4(1.0f, 0.0f, 0.0f, 0.0f) - pos);

	vec4 point1 = limHyperPlane(ro4, rd4, i1, j1, k1, m1);
	vec4 point2 = limHyperPlane(ro4, rd4, i2, j2, k2, m2);
	vec4 point3 = limHyperPlane(ro4, rd4, i3, j3, k3, m3);
	vec4 point4 = limHyperPlane(ro4, rd4, i4, j4, k4, m4);
	vec4 point5 = limHyperPlane(ro4, rd4, i5, j5, k5, m5);
	vec4 point6 = limHyperPlane(ro4, rd4, i6, j6, k6, m6);
	vec4 point7 = limHyperPlane(ro4, rd4, i7, j7, k7, m7);
	vec4 point8 = limHyperPlane(ro4, rd4, i8, j8, k8, m8);

	vec4 point = vec4(0.0f);
	vec4 n = vec4(0.0f);
	if (point1 != vec4(0.0f))
	{
		point = point1;
		n = n1;
	}
	if (point2 != vec4(0.0f) && (point == vec4(0.0f) || length(point2 - ro4) < length(point - ro4)))
	{
		point = point2;
		n = n2;
	}
	if (point3 != vec4(0.0f) && (point == vec4(0.0f) || length(point3 - ro4) < length(point - ro4)))
	{
		point = point3;
		n = n3;
	}
	if (point3 != vec4(0.0f) && (point == vec4(0.0f) || length(point3 - ro4) < length(point - ro4)))
	{
		point = point3;
		n = n3;
	}
	if (point4 != vec4(0.0f) && (point == vec4(0.0f) || length(point4 - ro4) < length(point - ro4)))
	{
		point = point4;
		n = n4;
	}
	if (point5 != vec4(0.0f) && (point == vec4(0.0f) || length(point5 - ro4) < length(point - ro4)))
	{
		point = point5;
		n = n5;
	}
	if (point6 != vec4(0.0f) && (point == vec4(0.0f) || length(point6 - ro4) < length(point - ro4)))
	{
		point = point6;
		n = n6;
	}
	if (point7 != vec4(0.0f) && (point == vec4(0.0f) || length(point7 - ro4) < length(point - ro4)))
	{
		point = point7;
		n = n7;
	}
	if (point8 != vec4(0.0f) && (point == vec4(0.0f) || length(point8 - ro4) < length(point - ro4)))
	{
		point = point8;
		n = n8;
	}

	if (point != vec4(0.0f))
		color = vec3(0.5f, 0.8f, 1.0f) * max(0.0f, dot(normalize(light), normalize(n)));

	gl_FragColor = vec4(color, 1.0f);
}