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
		void Init(int width, int height, void *fb);
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
		void SetRenderState(int _render_state) { m_render_state = _render_state; }
		void TransformUpdate();
		uint32_t **m_framebuffer;		// 像素缓存：m_framebuffer[y] 代表第 y行

	private:
		Transform m_transform;			// 坐标变换器
		int m_width;					// 窗口宽度
		int m_height;					// 窗口高度
		
		float **m_zbuffer;				// 深度缓存：m_zbuffer[y] 为第 y行指针
		uint32_t **m_texture;			// 纹理：同样是每行索引
		int m_texWidth;					// 纹理宽度
		int m_texHeight;				// 纹理高度
		float m_maxU;					// 纹理最大宽度：m_texWidth - 1
		float m_maxV;					// 纹理最大高度：m_texHeight - 1
		int m_render_state;				// 渲染状态
		uint32_t m_background;			// 背景颜色
		uint32_t m_foreground;			// 线框颜色

		bool m_blend;
		float m_blend_sfactor;
		float m_blend_dfactor;
		int m_cull;
	};
}
