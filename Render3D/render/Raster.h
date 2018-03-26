/*!
* @file Raster.h
* @date 2017/05/03 19:08
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief 光栅化阶段
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


#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

namespace Render3D
{
	/*!
	* @class Raster
	*
	* @brief 光栅化
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
		uint32_t **m_framebuffer;		// 像素缓存：m_framebuffer[y] 代表第 y行

	private:
		Transform transform;			// 坐标变换器
		int32_t *frameBuffer;             // 像素缓存
		float *zBuffer;				    // 深度缓存
		float *shadowBuffer;            // 阴影缓存
		int renderState;				// 渲染状态
		uint32_t background;			// 背景颜色
		uint32_t foreground;			// 线框颜色
		Base3D::Camera camera;
		Base3D::Material material;

		bool blend;
		float blendSrcFactor;
		float blendDestFactor;
		int cull;
	};
}
