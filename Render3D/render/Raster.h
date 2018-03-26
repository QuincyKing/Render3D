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
#include "Primitive.h"


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
		void SetBackground(uint32_t _background);
		void SetForeground(uint32_t _foreground);
		void SetFrameBuffer(uint32_t *_framebuffer);
		void SetZBuffer(float *_zbuffer);
		void SetShadowBuffer(float *_shadowbuffer);
		void SetCamera(Base3D::Camera _camera);
		void Destroy();
		void SetTexture(void *bits, long pitch, int w, int h);
		void Clear();
		void DrawPixel(int x, int y, uint32_t color);
		void DrawLine(int x1, int y1, int x2, int y2, uint32_t c);
		void ClipPolys(Vertex &v1, Vertex &v2, Vertex &v3, bool world);
		void DrawScanline(Scanline &scanline, Point &points, v2f *vfs);
		void SetRenderState(int _render_state) { renderState = _render_state; }
		uint32_t **m_framebuffer;		// ���ػ��棺m_framebuffer[y] ����� y��

	private:
		Transform transform;			// ����任��
		uint32_t *frameBuffer;             // ���ػ���
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
