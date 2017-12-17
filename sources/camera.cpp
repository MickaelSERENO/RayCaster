#include "Camera.h"

Camera::Camera(const Point& eye, const Point& center, const Vector& up, double focalDistance, int width, int height) : m_eye(eye), m_center(center), m_up(up), m_focalDistance(focalDistance), m_width(width), m_height(height)
{
	Vector G = (center - eye).normalized();
	m_A = G.cross(m_up).normalized();
	m_B = m_A.cross(G).normalized();
	m_M = eye + G * focalDistance;
}

Ray Camera::castRay(double x, double y)
{
	Point pixel = m_A * (-m_width / 2.0 + x) + m_B * (m_height/2.0-y) + m_M;
	return Ray(m_eye, (pixel - m_eye).normalized());
}