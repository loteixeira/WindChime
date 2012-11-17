// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "../Config.h"

class Vector3
{
public:
	float x, y, z;


	//
	// constructors
	//
	Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}


	//
	// operators
	//
	Vector3 operator-()
	{
		return *this * -1.0f;
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator+(float value) const
	{
		return Vector3(x + value, y + value, z + value);
	}

	Vector3 operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
		return *this;
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator-(float value) const
	{
		return Vector3(x - value, y - value, z - value);
	}

	Vector3 operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3 operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}

	Vector3 operator*(const Vector3& other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	Vector3 operator*(float value) const
	{
		return Vector3(x * value, y * value, z * value);
	}

	Vector3 operator*=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector3 operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3 operator/(const Vector3& other) const
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	Vector3 operator/(float value) const
	{
		return Vector3(x / value, y / value, z / value);
	}

	Vector3 operator/=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector3 operator/=(float value)
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	Vector3 operator=(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool operator==(const Vector3& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vector3& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}

	bool operator>(const Vector3& other)
	{
		return length() > other.length();
	}

	bool operator>(float value)
	{
		return length() > value;
	}

	bool operator>=(const Vector3& other)
	{
		return length() >= other.length();
	}

	bool operator>=(float value)
	{
		return length() >= value;
	}

	bool operator<(const Vector3& other)
	{
		return length() < other.length();
	}

	bool operator<(float value)
	{
		return length() < value;
	}

	bool operator<=(const Vector3& other)
	{
		return length() <= other.length();
	}

	bool operator<=(float value)
	{
		return length() <= value;
	}


	//
	// methods
	//
	Vector3 invert()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
		return *this;
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 length(float l)
	{
		normalize();
		x *= l;
		y *= l;
		z *= l;
		return *this;
	}

	Vector3 normalize()
	{
		float l = length();

		if (l != 0.0f)
		{
			x /= l;
			y /= l;
			z /= l;
		}

		return *this;
	}

	float distance(Vector3& other) const
	{
		return (other - *this).length();
	}

	Vector3 crossProduct(const Vector3& other) const
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	float dotProduct(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	string toString()
	{
		char buffer[256];
		string result = "[Vector3 x = ";

		sprintf(buffer, "%f", x);
		result += buffer;
		result += ", y = ";
		sprintf(buffer, "%f", y);
		result += buffer;
		result += ", z = ";
		sprintf(buffer, "%f", z);
		result += buffer;
		result += "]";

		return result;
	}
};


//
// useful constants
//
static const Vector3 zero(0.0f, 0.0f, 0.0f);
static const Vector3 gravity(0.0f, -0.0980665f, 0.0f);

#endif /* VECTOR3_H_ */
