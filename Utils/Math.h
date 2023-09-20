#pragma once

#include <iostream>
#include <assert.h>
#include <cmath>
#include <initializer_list>
#include <cstdlib>
#include <algorithm>

template<typename _scalar, typename _matT>
concept Scalar = requires(_scalar a, _matT matElem)
{
	{ a * matElem } ->  std::convertible_to<_matT>;
};
namespace Math {

	template<typename T, size_t N>
	struct RowView {

		RowView(const T* __val) {
			for (int i = 0; i < N; i++)
				view[i] = (T*)&__val[i];
		}

		T& operator[](int i) { return *view[i]; }
	private:
		T* view[N];


	};

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
		union {
			T arr[3];
			struct { T x, y, z; };
		};

		Vec3() : arr() {}
		Vec3(T a, T b, T c): x(a), y(b), z(c) {}
		Vec3(const Vec3<T>& other) { x = other.x; y = other.y; z = other.z; }

		Vec3<T> operator-() { return {-x, -y, -z}; }

		T& operator[](int i) { return arr[i]; }
		T operator[](int i) const {  return arr[i]; }

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

		float length() const { return sqrt(x * x + y * y + z * z); };

		Vec3 normalize()
		{
			float n = length();
			assert(n!=0);
			x = static_cast<double>(x) / n;
			y = static_cast<double>(y) / n;
			z = static_cast<double>(z) / n;

			return { x,y,z };
		}

		Vec3 normalize() const 
		{
			float n = length();
			float dx, dy, dz;
			assert(n != 0);
			dx = static_cast<double>(x) / n;
			dy = static_cast<double>(y) / n;
			dz = static_cast<double>(z) / n;

			return { dx,dy,dz };
		}





	};

	template <typename T = double>
	struct Vec4
	{
		union {
			T arr[4];
			struct { T x, y, z,w; };
		};

		Vec4() : x(0.f), y(0.f), z(0.f) , w(0.f) {}
		Vec4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}
		Vec4(const Vec4<T>& other) { x = other.x; y = other.y; z = other.z; w = other.w; }
		Vec4(const RowView<T,4>& rv) { x = rv[0]; y = rv[1]; z = rv[2]; w = rv[3]; }

		Vec4<T> operator-() { return { -x, -y, -z, -w }; }

		T& operator[](int i) { static_assert(i < 4); return arr[i]; }

		Vec4<T> operator*(const Vec4<T>& rhs)
		{
			return Vec4<T>{ x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
		}

		template<typename C>
		Vec4<T> operator*(C v)
		{
			return Vec4{ x * v, y * v, z * v, w * v };
		}

		Vec4<T> operator+(const Vec4<T>& rhs)
		{
			return Vec4{ x + rhs.x, y + rhs.y, z + rhs.z, w+rhs.w };
		}

		Vec4<T> operator-(const Vec4<T>& rhs)
		{
			return Vec4{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}

		Vec4 operator/(const Vec4& rhs)
		{
			return Vec4{ x / rhs.x, y / rhs.y, z / rhs.z, w/rhs.w };
		}

		void operator+=(const Vec4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
		}

		float length() { return sqrt(x * x + y * y + z * z + w*w); };

		Vec4 normalize()
		{
			float n = length();
			assert(n != 0);
			x = static_cast<double>(x) / n;
			y = static_cast<double>(y) / n;
			z = static_cast<double>(z) / n;
			w = static_cast<double>(w) / n;

			return { x,y,z,w };
		}





	};

	typedef Vec4<int> uVec4;
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
	T dot(const Math::Vec4<T>& lhs, const Math::Vec4<T>& rhs) {
		return {
			lhs.x * rhs.x +
			lhs.y * rhs.y +
			lhs.z * rhs.z +
			lhs.w * rhs.w
		};
	}

	template<typename T>
	T dot(const Math::Vec2<T>& lhs, const Math::Vec2<T>& rhs) {
		return {
				lhs.u * rhs.u +
				lhs.v * rhs.v 
		};
	}



	template<typename T=float> 
	struct Mat4x4 {

		union {
			T body[4][4];
			struct { T m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33; };
		};

		Mat4x4() : body() {}
		Mat4x4(std::initializer_list<T> l) {
			std::transform(l.begin(), l.end(), reinterpret_cast<T*>(body), [](auto p) {return p; });
		}
		Mat4x4(
			float _m00, float _m01, float _m02, float _m03,
			float _m10, float _m11, float _m12, float _m13,
			float _m20, float _m21, float _m22, float _m23,
			float _m30, float _m31, float _m32, float _m33
		)
		{
			m00 = _m00; m01 = _m01; m02 = _m02; m03 = _m03;
			m10 = _m10; m11 = _m11; m12 = _m12; m13	= _m13;
			m20 = _m20; m21 = _m21; m22 = _m22; m23	= _m23;
			m30 = _m30; m31 = _m31; m32 = _m32; m33 = _m33;
		}
		explicit Mat4x4(T fillVal) {
			m00 = fillVal; m01 = fillVal; m02 = fillVal; m03 = fillVal;
			m10 = fillVal; m11 = fillVal; m12 = fillVal; m13 = fillVal;
			m20 = fillVal; m21 = fillVal; m22 = fillVal; m23 = fillVal;
			m30 = fillVal; m31 = fillVal; m32 = fillVal; m33 = fillVal;
		}

		RowView<T, 4> operator[](int i) { return RowView<T, 4>(body[i]); }
		RowView<T, 4> operator[](int i) const { return RowView<T, 4>(body[i]); }

		Mat4x4<T> operator*(const Mat4x4<T>& other) const 
		{
			Mat4x4 res;

			res[0][0] = (m00 * other.m00) + (m01 * other.m10) + (m02 * other.m20) + (m03 * other.m30);
			res[0][1] = (m00 * other.m01) + (m01 * other.m11) + (m02 * other.m21) + (m03 * other.m31);
			res[0][2] = (m00 * other.m02) + (m01 * other.m12) + (m02 * other.m22) + (m03 * other.m32);
			res[0][3] = (m00 * other.m03) + (m01 * other.m13) + (m02 * other.m23) + (m03 * other.m33);

			res[1][0] = (m10 * other.m00) + (m11 * other.m10) + (m12 * other.m20) + (m13 * other.m30);
			res[1][1] = (m10 * other.m01) + (m11 * other.m11) + (m12 * other.m21) + (m13 * other.m31);
			res[1][2] = (m10 * other.m02) + (m11 * other.m12) + (m12 * other.m22) + (m13 * other.m32);
			res[1][3] = (m10 * other.m03) + (m11 * other.m13) + (m12 * other.m23) + (m13 * other.m33);

			res[2][0] = (m20 * other.m00) + (m21 * other.m10) + (m22 * other.m20) + (m23 * other.m30);
			res[2][1] = (m20 * other.m01) + (m21 * other.m11) + (m22 * other.m21) + (m23 * other.m31);
			res[2][2] = (m20 * other.m02) + (m21 * other.m12) + (m22 * other.m22) + (m23 * other.m32);
			res[2][3] = (m20 * other.m03) + (m21 * other.m13) + (m22 * other.m23) + (m23 * other.m33);

			res[3][0] = (m30 * other.m00) + (m31 * other.m10) + (m32 * other.m20) + (m33 * other.m30);
			res[3][1] = (m30 * other.m01) + (m31 * other.m11) + (m32 * other.m21) + (m33 * other.m31);
			res[3][2] = (m30 * other.m02) + (m31 * other.m12) + (m32 * other.m22) + (m33 * other.m32);
			res[3][3] = (m30 * other.m03) + (m31 * other.m13) + (m32 * other.m23) + (m33 * other.m33);

			return res;

		}

		Mat4x4<T> operator+(const Mat4x4<T>& other) {

			Mat4x4 res;
			for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) 
					res[i][j] += static_cast<T>(other[i][j]) + static_cast<T>(body[i][j]);
			

			return res;
		}


		template<typename S> requires Scalar<S, T>
		Math::Mat4x4<T> operator*(S scalar) const {

			Math::Mat4x4<T> res;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					res[i][j] = body[i][j] * scalar;


			return res;
		}

		template<typename S> requires Scalar<S, T>
		Math::Mat4x4<T> scale(S scalar) const {

			Math::Mat4x4<T> res = *this;
			for (int i = 0; i < 4; i++)
					res[i][i] *= scalar;

			return res;
		}

		template<typename T=float>
		static Mat4x4<T> identity() {
			return Mat4x4<T> 
			(
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
				
			);
		}





	};


template<typename T = float>
static Math::Mat4x4<T> rotate(const Math::Mat4x4<T>& m, T angle, const Math::Vec3<T>& v)
{
	T const a = angle;
	T const c = cos(a);
	T const s = sin(a);

	Math::Vec3<T> axis = v.normalize();
	Math::Vec3<T> temp = (axis * (T(1) - c));

	Math::Mat4x4<T> Rotate;
	Rotate[0][0] = c + temp[0] * axis[0];
	Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
	Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

	Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
	Rotate[1][1] = c + temp[1] * axis[1];
	Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

	Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
	Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
	Rotate[2][2] = c + temp[2] * axis[2];

	Math::Mat4x4<T> Result = m * Rotate;
	/*
	Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	Result[3] = m[3];
	*/
	return Result;
}

template<typename T=float>
static Math::Mat4x4<T> translate(const Math::Mat4x4<T>& m, const Math::Vec3<T>& v)
{
	Math::Mat4x4<T> Result(m);
	Result[3][0] += v[0];
	Result[3][1] += v[1];
	Result[3][2] += v[2];
	return Result;
}

} // -- END NAMESPACE Math::


template<typename S, typename T> requires Scalar<S, T>
static Math::Mat4x4<T> operator*(S scalar, const Math::Mat4x4<T>& mat) {

	Math::Mat4x4<T> res;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			res[i][j] =  scalar * mat[i][j];


	return res;
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

namespace Math {


	inline Math::Vec3<float> rot(Math::Vec3<float> vec, float angle)
	{

		float    fSinAngle = std::sin(angle);
		float    fCosAngle = std::cos(angle);

		return Math::Vec3<float>{vec.x, vec.y* fCosAngle + vec.z * fSinAngle, vec.y * -fSinAngle + vec.z * fCosAngle};
	}

	inline Math::Vec3<float> getWeights(Math::uVec2 p, Math::uVec2 v1, Math::uVec2 v2, Math::uVec2 v3) {

		float factor = 1.f / static_cast<float>((v2.v - v3.v) * (v1.u - v3.u) + (v3.u - v2.u) * (v1.v - v3.v));
		float W_1 = (v2.v - v3.v) * (p.u - v3.u) + (v3.u - v2.u) * (p.v - v3.v);
		float W_2 = (v3.v - v1.v) * (p.u - v3.u) + (v1.u - v3.u) * (p.v - v3.v);
		W_1 *= factor;
		W_2 *= factor;
		float W_3 = 1.f - W_1 - W_2;
		return { W_1, W_2, W_3 };

	}
}