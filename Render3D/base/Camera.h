#pragma once

#include "../math/Vector.h"
#include "../math/Matrix.h"

namespace Base3D
{
	typedef enum
	{
		perspective,
		orthographic
	}PROJECTION;

	class Camera
	{
	public:
		Math3D::Vector4 position;
		Math3D::Vector4 front;
		Math3D::Vector4 worldup;
		Math3D::Matrix44 viewMatrix;
		Math3D::Matrix44 projectionMatrix;
		Math3D::Matrix44 viewMatrixR;

		int width;
		int height;
		float fovy;
		float zNear;
		float zFar;

		float left;
		float right;
		float bottom;
		float top;

		bool dirty;
		PROJECTION projection;
		bool main;

		float aspect;

	public:
		void InitByEuler(float yaw, float pitch);
		void InitProjection();
		void Update();
	};
}