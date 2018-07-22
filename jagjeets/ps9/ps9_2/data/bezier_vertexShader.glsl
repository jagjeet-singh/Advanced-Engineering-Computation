attribute vec2 param;

uniform mat4 projection,modelView;
uniform vec3 ctp[16];
varying vec4 PosBeforeTrans;

vec3 CubicBezierCurve(in vec3 ctp[4],in float s)
{
vec3 a[3];
a[0] = vec3(ctp[0]*(1.0-s)+ctp[1]*s);
a[1] = vec3(ctp[1]*(1.0-s)+ctp[2]*s);
a[2] = vec3(ctp[2]*(1.0-s)+ctp[3]*s);
vec3 b[2];
b[0] = vec3(a[0]*(1.0-s)+a[1]*s);
b[1] = vec3(a[1]*(1.0-s)+a[2]*s);

return vec3(b[0]*(1.0-s)+b[1]*s);
}

vec3 CubicBezierSurface(in vec3 ctp[16],in float s,in float t)
{
vec3 a[4];
vec3 curr[4];
for (int i=0; i<4; i++)
{
	curr[0] = ctp[i*4];
	curr[1] = ctp[i*4+1];
	curr[2] = ctp[i*4+2];
	curr[3] = ctp[i*4+3];
	a[i] = CubicBezierCurve(curr, s);
}
//a[0] = CubicBezierCurve(ctp,s);
//a[1] = CubicBezierCurve(ctp+4,s);
//a[2] = CubicBezierCurve(ctp+8,s);
//a[3] = CubicBezierCurve(ctp+12,s);

return CubicBezierCurve(a,t);
}

void main()
{
	float s = param.x;
	float t = param.y;
	PosBeforeTrans = vec4(CubicBezierSurface(ctp,s,t), 1.0);
	vec4 PosAfterTrans = projection*modelView*PosBeforeTrans;
	gl_Position=PosAfterTrans;
}

