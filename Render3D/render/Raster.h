/*!
* @file Raster.h
* @date 2017/05/03 19:08
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief ��դ���׶�
*/
#pragma once

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <string>
#include "Transform.h"
#include "Primitive.h"
#include "../base/Camera.h"
#include "../base/Material.h"


#define RENDER_STATE_WIREFRAME      1		// ��Ⱦ�߿�
#define RENDER_STATE_TEXTURE        2		// ��Ⱦ����
#define RENDER_STATE_COLOR          4		// ��Ⱦ��ɫ

namespace Render3D
{
	/*!
	* @class Raster
	*
	* @brief ��դ��
	*/
	class Raster
	{
	public:
		void Init(Raster _raster);
		uint32_t ReadTexture(float u, float v);
		void Destroy();
		void SetTexture(void *bits, long pitch, int w, int h);
		void Clear();
		void DrawPixel(int x, int y, uint32_t color);
		void DrawLine(int x1, int y1, int x2, int y2, uint32_t c);
		void DrawScanline(Scanline scanline);

		void RenderTrap(Trapezoid &trap);
		void DrawPrimitive(const Vertex v1, const Vertex v2, const Vertex v3);

		void SetTransformWorld(const Math3D::Matrix44 _world);
		void SetTransformView(const Math3D::Matrix44 _view);
		void SetTransformProjection(const Math3D::Matrix44 _projection);
		void SetRenderState(int _render_state) { renderState = _render_state; }
		void TransformUpdate();
		uint32_t **m_framebuffer;		// ���ػ��棺m_framebuffer[y] ����� y��

	private:
		Transform transform;			// ����任��
		int32_t *frameBuffer;             // ���ػ���
		float *zBuffer;				    // ��Ȼ���
		float *shadowBuffer;            // ��Ӱ����
		int renderState;				// ��Ⱦ״̬
		uint32_t background;			// ������ɫ
		uint32_t foreground;			// �߿���ɫ
		Base3D::Camera camera;
		Base3D::Material material;

		bool blend;
		float blendSrcFactor;
		float blendDestFactor;
		int cull;
	};
}
