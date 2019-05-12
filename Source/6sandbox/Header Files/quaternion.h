#pragma once

#include <sb7.h>
#include <vmath.h>
#include <vector>
#include <Windows.h>
#include <string>

#define PI 3.14159265

// Euler hoeken
struct Euler
{
	float pitch = 0.0f; // x
	float yaw = 0.0f;   // y
	float roll = 0.0f;  // z
};

// quaternion
struct Quaternion
{
	float x = 0.0f; // i
	float y = 0.0f; // j
	float z = 0.0f; // k
	float w = 0.0f; // reëel

	// quaternionen vermenigvuldigen
	Quaternion operator*(Quaternion other)
	{
		float ax = this->x;
		float ay = this->y;
		float az = this->z;
		float aw = this->w;
		float bx = other.x;
		float by = other.y;
		float bz = other.z;
		float bw = other.w;

		Quaternion q;

		q.x = aw * bx + ax * bw + ay * bz - az * by;
		q.y = aw * by + ay * bw + az * bx - ax * bz;
		q.z = aw * bz + az * bw + ax * by - ay * bx;
		q.w = aw * bw - ax * bx - ay * by - az * bz;

		return q;
	}

	// quaternion omzetten naar rotatie matrix
	vmath::mat4 getRotationMatrix()
	{
		vmath::mat4 matrix;

		Quaternion q = *this; // kopie
		q.normalize();
		float x2 = q.x * q.x;
		float y2 = q.y * q.y;
		float z2 = q.z * q.z;
		float w2 = q.w * q.w;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.x * q.w;
		float wy = q.y * q.w;
		float wz = q.z * q.w;

		// kolommen gaan van boven naar beneden, rijen van links naar rechts

		// eerste rij
		matrix[0][0] = 1 - 2 * (y2 + z2);
		matrix[0][1] = 2 * (xy - wz);
		matrix[0][2] = 2 * (wy + xz);
		matrix[0][3] = 0;

		// tweede rij 
		matrix[1][0] = 2 * (xy + wz);
		matrix[1][1] = 1 - 2 * (x2 + z2);
		matrix[1][2] = 2 * (yz - wx);
		matrix[1][3] = 0;

		// derde rij
		matrix[2][0] = 2 * (xz - wy);
		matrix[2][1] = 2 * (wx + yz);
		matrix[2][2] = 1 - 2 * (x2 + y2);
		matrix[2][3] = 0;

		// vierde rij
		matrix[3][0] = 0;
		matrix[3][1] = 0;
		matrix[3][2] = 0;
		matrix[3][3] = 1;

		return matrix;
	}

	// quaternion normaliseren
	void normalize()
	{
		Quaternion *q = this; // origineel
		float magnitude = sqrt(q->x*q->x + q->y*q->y + q->z*q->z + q->w*q->w);
		q->x /= magnitude;
		q->y /= magnitude;
		q->x /= magnitude;
		q->w /= magnitude;
	}

	// geconjugeerde berekenen
	Quaternion conjugate()
	{
		Quaternion q = *this; // kopie
		q.x = -q.x;
		q.y = -q.y;
		q.z = -q.z;
		q.w = q.w;
		return q;
	}

	vmath::vec4 rotateVectorByQuat(vmath::vec4 vec)
	{
		Quaternion q = *this; // kopie
		q.normalize();
		Quaternion p;
		p.x = vec[0];
		p.y = vec[1];
		p.z = vec[2];
		p.w = 0;

		Quaternion r = q * p * q.conjugate();
		return vmath::vec4(r.x, r.y, r.z, 1.0f);
		
		/* sneller alternatief voor vector rotatie m.b.v. quaternionen

		vmath::vec3 u(q.x, q.y, q.z); // vector component van rotatie quaternion
		float s = q.w; // scalair component van rotatie quaternion

		vmath::vec3 v(vec[0], vec[1], vec[2]); // input vector (vierdelig) omzetten naar driedelige vector

		// nieuwe positie r berekenen
		vmath::vec3 r;
		r = 2.0f * vmath::dot(u, v) * u
			+ (s*s - dot(u, u)) * v
			+ 2.0f * s * vmath::cross(u, v);

		return vmath::vec4(r[0], r[1], r[2], 1.0f); 
		*/
	}
	
	// as-hoek representatie omzetten naar quaternion
	void AxisAngleToQuat(vmath::vec3 axis, float angle_degrees)
	{
		float angle_radians = angle_degrees * 180 / PI;

		vmath::normalize(axis);
		Quaternion *q = this; // origineel

		float sine = sin(angle_radians / 2);
		q->x = axis[0] * sine;
		q->y = axis[1] * sine;
		q->z = axis[2] * sine;
		q->w = cos(angle_radians / 2);
		q->normalize();
	}

	// quaternion maken op basis van Euler hoeken
	void EulerToQuat(Euler angles)
	{
		float cosy = cos((angles.roll * PI / 180) * 0.5);
		float siny = sin((angles.roll * PI / 180) * 0.5);
		float cosz = cos((angles.pitch * PI / 180) * 0.5);
		float sinz = sin((angles.pitch * PI / 180) * 0.5);
		float cosx = cos((angles.yaw * PI / 180) * 0.5);
		float sinx = sin((angles.yaw * PI / 180) * 0.5);

		Quaternion *q = this;
		q->w = cosy * cosx * cosz + siny * sinx * sinz; // w
		q->x = cosy * cosx * sinz - siny * sinx * cosz; // x
		q->y = siny * cosx * sinz + cosy * sinx * cosz; // y
		q->z = siny * cosx * cosz - cosy * sinx * sinz; // z
		q->normalize();
	}

	// Euler hoeken verkrijgen uit quaternion (experimentele fase)
	Euler QuatToEuler()
	{
		Euler angles;
		Quaternion q = *this;
		float x2 = q.x * q.x;
		float y2 = q.y * q.y;
		float z2 = q.z * q.z;
		angles.roll = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (y2 * z2)) * 180 / PI;
		angles.pitch = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (x2 * y2)) * 180 / PI;
		angles.yaw = asin(2 * (q.w * q.y - q.z * q.x)) * 180 / PI;

		return angles;
	}
};