#include "QMath.hpp"



#ifndef DEF_0

void __forceinline QMath::VectorAngles(const vec3_t value1, vec3_t angles)
{
	static float	forward;
	static float	yaw, pitch;

	if (value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if (value1[2] > 0)
		{
			pitch = 90;
		}
		else
		{
			pitch = 270;
		}
	}
	else
	{
		if (value1[0])
		{
			yaw = (atan2(value1[1], value1[0]) * 180 / M_PI);
		}
		else if (value1[1] > 0)
		{
			yaw = 90;
		}
		else
		{
			yaw = 270;
		}
		if (yaw < 0)
		{
			yaw += 360;
		}

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (atan2(value1[2], forward) * 180 / M_PI);
		if (pitch < 0)
		{
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}



void __forceinline QMath::AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	static float		angle; // ye too...
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs // k

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);


	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;

	right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
	right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
	right[2] = -1 * sr*cp;

	up[0] = (cr*sp*cy + -sr*-sy);
	up[1] = (cr*sp*sy + -sr*cy);
	up[2] = cr*cp;

}


void __forceinline QMath::AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right)
{
	static float		angle; // ye too...
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs // k

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);


	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;

	right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
	right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
	right[2] = -1 * sr*cp;
}




void __forceinline QMath::AngleVectors(const vec3_t angles, vec3_t forward)
{
	static float		angle; // ye too...
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs // k

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);


	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;
}


#endif



