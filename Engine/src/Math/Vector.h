// VoxelEngine - Engine
// Vector.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:48
// Last update: 03-09-2022 17:15

#pragma once

namespace VoxelEngine
{
	struct Vec2
	{
		union { float x, r; };
		union { float y, g; };

		Vec2()
			: x(0.0f), y(0.0f) {}

		Vec2(float x, float y)
			: x(x), y(y) {}

		// Operators
		Vec2& operator+=(const Vec2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vec2& operator-=(const Vec2& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vec2& operator*=(const Vec2& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		Vec2& operator/=(const Vec2& v)
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}

		// Prefix increment
		Vec2& operator++()
		{
			x++;
			y++;
			return *this;
		}

		// Postfix increment
		Vec2& operator++(int)
		{
			Vec2 v = *this;
			operator++();
			return v;
		}

		// Prefix decrement
		Vec2& operator--()
		{
			x--;
			y--;
			return *this;
		}

		// Postfix decrement
		Vec2& operator--(int)
		{
			Vec2 v = *this;
			operator--();
			return v;
		}
	};

	struct Vec3
	{
		union { float x, r; };
		union { float y, g; };
		union { float z, b; };

		Vec3()
			: x(0.0f), y(0.0f), z(0.0f) {}

		Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}

		// Operators
		Vec3& operator+=(const Vec3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vec3& operator-=(const Vec3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vec3& operator*=(const Vec3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Vec3& operator/=(const Vec3& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		// Prefix increment
		Vec3& operator++()
		{
			x++;
			y++;
			z++;
			return *this;
		}

		// Postfix increment
		Vec3& operator++(int)
		{
			Vec3 v = *this;
			operator++();
			return v;
		}

		// Prefix decrement
		Vec3& operator--()
		{
			x--;
			y--;
			z--;
			return *this;
		}

		// Postfix decrement
		Vec3& operator--(int)
		{
			Vec3 v = *this;
			operator--();
			return v;
		}
	};

	struct Vec4
	{
		union { float x, r; };
		union { float y, g; };
		union { float z, b; };
		union { float w, a; };

		Vec4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		Vec4(float x, float y, float z)
			: x(x), y(y), z(z), w(0.0f) {}

		Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w) {}

		// Operators
		Vec4& operator+=(const Vec4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		Vec4& operator-=(const Vec4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		Vec4& operator*=(const Vec4& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		Vec4& operator/=(const Vec4& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}

		// Prefix increment
		Vec4& operator++()
		{
			x++;
			y++;
			z++;
			w++;
			return *this;
		}

		// Postfix increment
		Vec4& operator++(int)
		{
			Vec4 v = *this;
			operator++();
			return v;
		}

		// Prefix decrement
		Vec4& operator--()
		{
			x--;
			y--;
			z--;
			w--;
			return *this;
		}

		// Postfix decrement
		Vec4& operator--(int)
		{
			Vec4 v = *this;
			operator--();
			return v;
		}
	};
}
