#pragma once

#include <iostream>
#include <assert.h>
#include <cmath>

namespace Math {



	template <typename T=double>
	struct Vec2 
	{
		T u;
		T v;

		Vec2() : u(0.f), v(0.f) {}
		Vec2(T a, T b) : u(a), v(b) {}

		Vec2 operator*(const Vec2& rhs)
		{
			return Vec2{ u * rhs.u, v * rhs.v };
		}

		Vec2 operator+(const Vec2& rhs)
		{
			return Vec2{ u + rhs.u, v + rhs.v };
		}
		Vec2 operator-(const Vec2& rhs)
		{
			return Vec2{ u - rhs.u, v - rhs.v };
		}
		Vec2 operator/(const Vec2& rhs)
		{
			return Vec2{ u / rhs.u, v / rhs.v };
		}

		Vec2 normalize() 
		{
			assert(u && v);
			float n = u * u + v * v;
			u = u / n;
			v = v / n;
			return { u,v };
		}


	};

	template <typename T=double>
	struct Vec3
	{
		T x;
		T y;
		T z;

		Vec3() : x(0.f), y(0.f), z(0.f) {}
		Vec3(T a, T b, T c): x(a), y(b), z(c) {}
		Vec3(const Vec3<T>& other) { x = other.x; y = other.y; z = other.z; }

		Vec3 operator*(const Vec3& rhs) 
		{
			return Vec3{ x * rhs.x, y * rhs.y, z * rhs.z };
		}

		template<typename C>
		Vec3 operator*(C v)
		{
			return Vec3{ x * v, y * v, z *v };
		}

		Vec3 operator+(const Vec3& rhs)
		{
			return Vec3{ x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vec3 operator-(const Vec3& rhs)
		{
			return Vec3{ x - rhs.x, y - rhs.y, z - rhs.z };
		}

		Vec3 operator/(const Vec3& rhs)
		{
			return Vec3{ x / rhs.x, y / rhs.y, z / rhs.z };
		}

		void operator+=(const Vec3& rhs) 
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		}

		float length() { return sqrt(x * x + y * y + z * z); };

		Vec3 normalize()
		{
			float n = length();
			assert(n!=0);
			x = static_cast<double>(x) / n;
			y = static_cast<double>(y) / n;
			z = static_cast<double>(z) / n;

			return { x,y,z };
		}





	};

	typedef Vec3<int> uVec3;
	typedef Vec2<int> uVec2;

	template<typename T>
	Math::Vec3<T> cross(const Math::Vec3<T>& lhs, const Math::Vec3<T>& rhs) {
		return {

			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x

		};
	}

	template<typename T>
	T dot(const Math::Vec3<T>& lhs, const Math::Vec3<T>& rhs) {
			return {
				lhs.x * rhs.x +
				lhs.y * rhs.y +
				lhs.z * rhs.z
		};
	}

	template<typename T>
	T dot(const Math::Vec2<T>& lhs, const Math::Vec2<T>& rhs) {
		return {
				lhs.u * rhs.u +
				lhs.v * rhs.v 
		};
	}

}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Math::Vec3<T>& vec)
{
	os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Math::Vec2<T>& vec)
{
	os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return os;
}

// -- Others

inline Math::Vec3<float> rot(Math::Vec3<float> vec, float angle)
{

	float    fSinAngle = std::sin(angle);
	float    fCosAngle = std::cos(angle);

	return Math::Vec3<float>{vec.x, vec.y* fCosAngle + vec.z * fSinAngle, vec.y * -fSinAngle + vec.z * fCosAngle};
}