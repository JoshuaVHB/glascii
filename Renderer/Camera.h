#pragma once

#include "../Utils/Math.h"
#include "Console.h"

template <class _Proj, typename _matType=float>
concept Projection = requires(_Proj p)
{
	{p.getProjMatrix() } -> std::convertible_to<Math::Mat4x4<_matType>>;
};

struct OrthographicProjection {

	float top, bot;
	float left, right;
	float znear, zfar;

	OrthographicProjection(float l, float r, float t, float b, float zn, float zf) 
	{
		left = l;
		right = r;
		top = t;
		bot = b;
		znear = zn;
		zfar = zf;

	}

	Math::Mat4x4<float> getProjMatrix() const 
	{
		Math::Mat4x4<float> M = Math::Mat4x4<float>::identity();
		M[0][0] = 2.F / (right - left);
		M[1][1] = 2.F / (top - bot);
		M[2][2] = -2.F / (zfar - znear);

		M[3][0] = -(right + left) / (right - left);
		M[3][1] = -(top + bot) / (top - bot);
		M[3][2] = -(zfar + znear) / (zfar - znear);

		return M;
	}


};

class Camera {

private:

	Math::Mat4x4<float> m_projMatrix;
	Math::Mat4x4<float> m_viewMatrix;
	Math::Mat4x4<float> m_VPMatrix;

	Math::Vec3<float> m_pos;
	Math::Vec3<float> m_target;

	struct Axes {
		Math::Vec3<float> left;
		Math::Vec3<float> up;
		Math::Vec3<float> forward;
	} m_axes;

	struct Angles {
		float yaw = 0.f;
		float pitch = 0.f;
		float roll = 0.f;
	} m_angles ;

public:

	template<Projection projType=OrthographicProjection>
	Camera(projType projection) 
	{
		m_projMatrix = projection.getProjMatrix();
		computeViewProjMatrix();

	}

	template<Projection projType>
	static Math::Mat4x4<float> getProjMatrix(const projType& proj) { return proj.getProjMatrix(); }
	Math::Mat4x4<float> getViewProjMatrix() const { return m_VPMatrix; }

	void computeViewMatrix()
	{
		m_viewMatrix = Math::rotate(Math::Mat4x4<float>(1.F), -m_angles.pitch, { 1, 0, 0 });
		m_viewMatrix = Math::rotate(m_viewMatrix, -m_angles.yaw, { 0, 1, 0 });
		m_viewMatrix = Math::translate(m_viewMatrix, -m_pos);

	}


	void computeViewProjMatrix() {

		
		computeViewMatrix();
		m_VPMatrix = m_projMatrix * m_viewMatrix;
	}

	void lookAt(const Math::Vec3<float>& target) {
		m_target = target;
		m_axes.forward = Math::Vec3<float>({ target.x - m_pos.x, target.y - m_pos.y, target.z - m_pos.z }).normalize();
		m_axes.left = cross(m_axes.forward, {0.f,1.f,0.f}).normalize();
		m_axes.up = cross(m_axes.forward, m_axes.left).normalize();
	}

	void updateCam(float deltaTime) {
		lookAt(m_target);

	}

	void move(const Math::Vec3<float>& delta) {
		m_pos += delta;
	}

	void setTarget(Math::Vec3<float> target) { m_target = target; }
	Math::Vec3<float> getPosition() const { return m_pos; }
	Math::Vec3<float> getForward() const { return m_axes.forward; }

};

class OrthographicCamera {

	Math::Vec3<float> UP{ 0.f, 1.f, 0.f };

public:

	OrthographicCamera() {
		updateCam(0);
	}

	void lookAt(const Math::Vec3<float>& target) {
		m_forward = Math::Vec3<float>({ target.x - m_position.x, target.y - m_position.y, target.z - m_position.z }).normalize();
		m_left = cross(m_forward, UP).normalize();
		m_up = cross(m_forward, m_left).normalize();
	}

	void lookAtTarget() {
		m_forward = Math::Vec3<float>({ m_target.x - m_position.x, m_target.y - m_position.y, m_target.z - m_position.z }).normalize();
		m_left = cross(m_forward, UP).normalize();
		m_up = cross(m_forward, m_left).normalize();
	}

	Math::Vec2<float> toCamCoords(Math::Vec3<float> p) const {
		return {
		  dot(p, m_left) - dot(m_position, m_left),
		  dot(p, m_up) - dot(m_position, m_up),
		};
	};

	Math::uVec2 toScreenCoords(Math::Vec2<float> p) const  {
		Math::uVec2 win = { Console::s_WindowSize.w, Console::s_WindowSize.h };
		return {
		  static_cast<int>(p.u * scaleFactor + win.u * .5f),
		  static_cast<int>(p.v * scaleFactor + win.v * .5f)
		};
	};

	void updateCam(float deltaTime ,bool animateCamera=true) {

		if (animateCamera) {
			t += deltaTime;
			float l = 5;
			m_position.x = m_target.x + std::cos(t) * l;
			m_position.y = 3;
			m_position.z = m_target.z + std::sin(t) * l;
		}
		// orthographic projection
		lookAt(m_target);
	};

	void move(const Math::Vec3<float>& delta) {
		m_position += delta;
	}

	void setTarget(Math::Vec3<float> target) { m_target = target; }

	Math::Vec3<float> getPosition() const { return m_position; }
	Math::Vec3<float> getForward() const { return m_forward; }

	void setScale(float f) { scaleFactor = f; }
	float scaleFactor = 100;

private:
	float t = 0;
	Math::Vec3<float> m_target{ 0, 0, 0 };
	Math::Vec3<float> m_left{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_up{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_position{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_forward{ 0.f, 1.f, 0.f };

};

