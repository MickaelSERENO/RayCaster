#ifndef DEF_CAMERA
#define DEF_CAMERA

#include "triple.h"
#include "light.h"

class Camera
{
	public:
		Camera(const Point& eye, const Point& center, const Vector& up, double focalDistance, int width, int height);
		Ray castRay(double x, double y);
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
	private:
		Point m_eye;
		Point m_center;
		Vector m_up;
		Vector m_A;
		Vector m_B;
		Vector m_M;
		double m_focalDistance;
		int m_width;
		int m_height;
};

#endif