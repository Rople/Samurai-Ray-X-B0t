/*
All rights of this file reserved to quake engine source
*/

#include <math.h>


typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];

// returns the sign by 1 according to float or int...
#define GetSign(x) (x ? ((*(int*)&x >> 31) | 0x1) : 0)


#define Normalize2DAngle360Fast(x)			\
{											\
	while (x[0] > 360.0f) x[0] -= 360.0f;	\
	while (x[1] > 360.0f) x[1] -= 360.0f;	\
	while (x[0] < -360.0f) x[0] += 360.0f;	\
	while (x[1] < -360.0f) x[1] += 360.0f;  \
}
// q_shared.h

#define	ANGLE2SHORT(x)	((int)((x)*65536/360) & 65535)
#define	SHORT2ANGLE(x)	((x)*(360.0/65536))

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define Square(x)				((x)*(x))
#define VectorDistance(a, b)	( sqrt( ( Square(b[0] - a[0]) + Square(b[1] - a[1]) + Square(b[2] - a[2]) ) ) )
#define VectorNormalizeFast(v)		{ float ilen = sqrt( DotProduct(v, v) );  (v)[0] *= (ilen), (v)[1] *= (ilen), (v)[2] *= (ilen); }
// angle indexes
#define	PITCH				0		// up / down
#define	YAW					1		// left / right
#define	ROLL				2		// fall over
#define M_PI		3.14159265358979323846f	// matches value in gcc v2 math.h

// q_math.c


#pragma once

class QMath
{
public:
	static void __forceinline AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
	static void __forceinline AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right);
	static void __forceinline AngleVectors(const vec3_t angles, vec3_t forward);
	static void __forceinline VectorAngles(const vec3_t value1, vec3_t angles);
};


#include "QMath.cpp"
#define DEF_0

