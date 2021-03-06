#include "vec4f.h"
#include "vec3f.h"
#include "matrix4.h"
#include <math.h>

using namespace std;

namespace Epoch
{

#define EPSILON 0.001f

	vec4f::vec4f(const vec4f& _copy)
	{
		for (int i = 0; i < 4; ++i)
			xyzw[i] = _copy.xyzw[i];
	}

	vec4f::vec4f(const vec3f& _copy)
	{
		for (int i = 0; i < 3; ++i)
			xyzw[i] = _copy.xyz[i];
		w = 1;
	}

	bool vec4f::operator==(const vec4f& _other) const
	{
		for (int i = 0; i < 4; ++i)
			if (fabsf(xyzw[i] - _other.xyzw[i]) > EPSILON)
				return false;
		return true;
	}

	vec4f& vec4f::operator=(const vec4f& _other)
	{
		if (*this != _other)
			for (int i = 0; i < 4; ++i)
				xyzw[i] = _other.xyzw[i];
		return *this;
	}

	vec4f vec4f::operator^(const vec4f& _other) const
	{
		vec4f temp;
		temp.x = (y * _other.z) - (z * _other.y);
		temp.y = (z * _other.x) - (x * _other.z);
		temp.z = (x * _other.y) - (y * _other.x);
		temp.w = 1;
		return temp;
	}

	vec4f vec4f::operator*(const matrix4& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = (xyzw[0] * _other.first.xyzw[i]) +
						   (xyzw[1] * _other.second.xyzw[i]) -
						   (xyzw[2] * _other.third.xyzw[i]) +
						   (xyzw[3] * _other.fourth.xyzw[i]);

		return temp;
	}

	vec4f& vec4f::operator*=(const matrix4& _other)
	{
		*this = *this * _other;
		return *this;
	}

	float vec4f::operator*(const vec4f& _other) const
	{
		return x * _other.x + y * _other.y + z * _other.z + w * _other.w;
	}

	vec4f vec4f::operator*(const float& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] * _other;
		return temp;
	}

	vec4f& vec4f::operator*=(const float& _other)
	{
		*this = *this * _other;
		return *this;
	}

	vec4f vec4f::operator/(const float& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] / _other;
		return temp;
	}

	vec4f& vec4f::operator/=(const float& _other)
	{
		*this = *this / _other;
		return *this;
	}

	vec4f vec4f::operator-() const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] * -1.0f;
		return temp;
	}

	vec4f vec4f::operator-(const vec4f& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] - _other.xyzw[i];
		return temp;
	}

	vec4f vec4f::operator-(const float& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] - _other;
		return temp;
	}

	vec4f& vec4f::operator-=(const vec4f& _other)
	{
		*this = *this - _other;
		return *this;
	}

	vec4f& vec4f::operator-=(const float& _other)
	{
		*this = *this - _other;
		return *this;
	}

	vec4f vec4f::operator+(const float& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] + _other;
		return temp;
	}

	vec4f vec4f::operator+(const vec4f& _other) const
	{
		vec4f temp;
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] + _other.xyzw[i];
		return temp;
	}

	vec4f& vec4f::operator+=(const float& _other)
	{
		*this = *this + _other;
		return *this;
	}

	vec4f& vec4f::operator+=(const vec4f& _other)
	{
		*this = *this + _other;
		return *this;
	}

	float& vec4f::operator[](const unsigned int _index)
	{
		if (_index < 4)
			return xyzw[_index];
		return xyzw[0];
	}

	vec4f vec4f::Cross(const vec4f& _other) const
	{
		return *this ^ _other;
	}

	float vec4f::Dot(const vec4f& _other) const
	{
		return *this * _other;
	}

	float vec4f::Magnitude() const
	{
		return sqrtf(SquaredMagnitude());
	}

	float vec4f::SquaredMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	vec4f vec4f::Normalize() const
	{
		vec4f temp;
		float norm = 1 / Magnitude();
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] * norm;
		return temp;
	}

	vec4f vec4f::Reflect(const vec4f& _other) const
	{
		vec4f temp, N;
		N = _other.Normalize();
		float reflect = -2 * (*this * N);
		temp = N * reflect - *this;
		return temp;
	}

	vec4f & vec4f::Set(const float _x, const float _y, const float _z, const float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
		return *this;
	}

}