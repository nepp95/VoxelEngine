// VoxelEngine - Engine
// Vector.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:48
// Last update: 01-09-2022 20:33

#pragma once

namespace VoxelEngine
{
	// To achieve xyz / rgb having the same values, both are initialized to the same value.
	// Every operator is overloaded to change both values.
	struct Vec2
	{
		float x, y;

		Vec2();

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
	};

	struct Vec3
	{
		union
		{
			struct { float x, y, z; } xyz;
			struct { float r, g, b; } rgb;
		};

		Vec3();

		Vec3(float x, float y, float z)
			: xyz{ x, y, z } {}
		Vec3(float r, float g, float b)
			: rgb{ r, g, b } {}

		// Operators
		Vec3& operator+=(const Vec3& v)
		{
			X += v.X;
			Y += v.Y;
			Z += v.Z;
			return *this;
		}

		Vec3& operator-=(const Vec3& v)
		{
			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			return *this;
		}

		Vec3& operator*=(const Vec3& v)
		{
			X *= v.X;
			Y *= v.Y;
			Z *= v.Z;
			return *this;
		}

		Vec3& operator/=(const Vec3& v)
		{
			X /= v.X;
			Y /= v.Y;
			Z /= v.Z;
			return *this;
		}
	};

	struct Vec4
	{
		union
		{
			struct { float x, y, z, w; } xyzw;
			struct { float r, g, b, a; } rgba;
		};

		Vec4();

		Vec4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {}

		// Operators
		Vec4& operator+=(const Vec4& v)
		{
			X += v.X;
			Y += v.Y;
			Z += v.Z;
			W += v.W;
			return *this;
		}

		Vec4& operator-=(const Vec4& v)
		{
			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			W -= v.W;
			return *this;
		}

		Vec4& operator*=(const Vec4& v)
		{
			X *= v.X;
			Y *= v.Y;
			Z *= v.Z;
			W *= v.W;
			return *this;
		}

		Vec4& operator/=(const Vec4& v)
		{
			X /= v.X;
			Y /= v.Y;
			Z /= v.Z;
			W /= v.W;
			return *this;
		}
	};
}
