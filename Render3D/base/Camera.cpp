#include "Camera.h"
#include <math.h>
#include "../math/MathUtil.h"

namespace Base3D
{
	void Camera::InitByEuler(float yaw, float pitch)
	{
		front.X() = sin(angleToRadian(yaw)) * cos(angleToRadian(pitch));
		front.Y() = -sin(angleToRadian(pitch));
		front.Z() = cos(angleToRadian(yaw)) * cos(angleToRadian(pitch));
		front = Normalize(front);
	}

	void Camera::InitProjection()
	{
		if (projection == perspective)
			MatrixSetPerspective(projectionMatrix, fovy, aspect, zNear, zFar);
		else if (projection == orthographic)
			MatrixSetOrtho(projectionMatrix, left, right, bottom, top, zNear, zFar);
	}

	void Camera::Update()
	{
		Math3D::Vector4 right, at, up, front = this->front;
		right = Cross(worldup, front);
		Normalize(right);
		up = Cross(front, right);
		Normalize(up);
		at = position + front;

		MatrixSetLookat(viewMatrix, position, at, up);
		Normalize(front);
		MatrixSetAxis(viewMatrixR, right, up, front, position);
	}
}