#pragma once

#include "../Utils/Math.h"
#include "Console.h"



class OrthographicCamera {

	Math::Vec3<float> UP{ 0.f, 1.f, 0.f };

public:

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

