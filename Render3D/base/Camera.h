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
		void operator =(Camera _camera)
		{
			position = _camera.position; width = _camera.width; right = _camera.right;  aspect = _camera.aspect;
			front = _camera.front;   height = _camera.height;  bottom = _camera.bottom;
			worldup = _camera.worldup;  fovy = _camera.fovy;  top = _camera.top;
			viewMatrix = _camera.viewMatrix;  zNear = _camera.zNear;  dirty = _camera.dirty;
			projectionMatrix = _camera.projectionMatrix; zFar = _camera.zFar; projection = _camera.projection;
			viewMatrixR = _camera.viewMatrixR;  left = _camera.left; main = _camera.main;
		}


		void InitByEuler(float yaw, float pitch);
		void InitProjection();
		void Update();
	};
}