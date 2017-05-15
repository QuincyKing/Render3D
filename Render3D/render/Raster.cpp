#include "Raster.h"

namespace Render3D
{
	/*!
	* 函数全名 Render3D::Raster::Init
	*
	* @brief 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
	*
	* @return void
	*
	* @param int width 屏幕宽度
	* @param int height 屏幕高度
	* @param void * fb 帧缓冲
	*/
	void Raster::Init(int width, int height, void *fb)
	{
		//1. framebuffer 为 height * sizeof(void *)
		//2. zbuffer 为height * sizeof(void *)
		//3. texture 为1024 * sizeof(void *)
		//4. framebuf行 为 4 * width * height
		//5. zbuf行 为4 * width * height
		//6. texure 为64
		m_width = width;
		m_height = height;
		m_texHeight = 2;
		m_texWidth = 2;
		m_maxU = m_texWidth - 1.0f;
		m_maxV = m_texHeight - 1.0f;
		m_render_state = RENDER_STATE_WIREFRAME;
		m_background = 0x0c0c0c;
		m_foreground = 0;
		int need = 2 * height * sizeof(void *) + 1024 * sizeof(void *) + 8 * width * height + 64;
		char *ptr = (char *)malloc(need);
		assert(ptr);
		m_framebuffer = (uint32_t **)ptr;
		m_zbuffer = (float **)ptr + height * sizeof(void *);
		m_texture = (uint32_t **)ptr + 2 * height *sizeof(void *);
		ptr += 2 * height * sizeof(void *) + 1024 * sizeof(void *);
		char *framebuf = (char *)ptr;
		char *zbuf = (char *)ptr + width * height * 4;
		ptr += 8 * width * height;
		if (fb != NULL) 
			framebuf = (char*)fb;
		//动态申请framebuffer和zbuffer的二维数组
		for (int j = 0; j < height; j++)
		{
			m_framebuffer[j] = (uint32_t *)(framebuf + j * width * 4);
			m_zbuffer[j] = (float *)(zbuf + j * width * 4);
		}
		m_texture[0] = (uint32_t *)ptr;
		m_texture[1] = (uint32_t *)(ptr + 64);
		memset(m_texture[0], 0, 64);
		//初始化几何矩阵
		m_transform.Init(width, height);
	}

	/*!
	* 函数全名 Render3D::Raster::Destroy
	*
	* @brief 删除设备
	*
	* @return void
	*
	*/
	void Raster::Destroy()
	{
		if (m_framebuffer != NULL)
			free(m_framebuffer);
		m_framebuffer = NULL;
		m_zbuffer = NULL;
		m_texture = NULL;
	}

	/*!
	* 函数全名 Render3D::Raster::SetTexture
	*
	* @brief 设置当前纹理
	*
	* @return void
	*
	* @param void * bits 纹理
	* @param long pitch 纹理条长度
	* @param int w 纹理宽
	* @param int h 纹理长
	*/
	void Raster::SetTexture(void *bits, long pitch, int w, int h)
	{
		m_texHeight = h;
		m_texWidth = w;
		m_maxU = w - 1.0f;
		m_maxV = h - 1.0f;
		assert(w <= 1024 && h <= 1024);
		char *ptr = (char *)bits;
		for (int i = 0; i < h; i++, ptr += pitch)
		{
			m_texture[i] = (uint32_t *)(ptr);
		}
	}

	/*!
	* 函数全名 Render3D::Raster::Clear
	*
	* @brief 清空framebuffer和zbuffer
	*
	* @return void
	*
	*/
	void Raster::Clear()
	{
		int y, x, height = m_height;
		for (y = 0; y < m_height; y++)
		{
			uint32_t *frame_dst = m_framebuffer[y];
			float *z_dst = m_zbuffer[y];
			for (x = m_width; x > 0; x--)
			{
				frame_dst[x] = m_background;
				z_dst[x] = 0.0f;
			}
		}
	}

	/*!
	* @function Render3D::Raster::TextureRead
	*
	* @brief 根据uv坐标读取纹理
	*
	* @return uint32_t
	*
	* @param float u
	* @param float v
	*/
	uint32_t Raster::ReadTexture(float u, float v)
	{
		u *= m_maxU;
		v *= m_maxV;
		int x = (int)(u + 0.5f);
		int y = (int)(v + 0.5f);
		CMID(x, 0, m_texWidth - 1);
		CMID(y, 0, m_texHeight - 1);
		return m_texture[x][y];
	}

	/*!
	* 函数全名 Render3D::Raster::DrawPixel
	*
	* @brief 画点
	*
	* @return void
	*
	* @param int x 像素x坐标
	* @param int y 像素y坐标
	* @param uint32_t color 像素颜色
	*/
	void Raster::DrawPixel(int x, int y, uint32_t color)
	{
		if (x < m_width && y < m_height)
			m_framebuffer[y][x] = color;
	}

	/*!
	* 函数全名 Render3D::Raster::DrawLine
	*
	* @brief DDS算法绘制线段
	*
	* @return void
	*
	* @param int x1
	* @param int y1
	* @param int x2
	* @param int y2
	* @param uint32_t c
	*/
	void Raster::DrawLine(int x1, int y1, int x2, int y2, uint32_t c)
	{
		//1. 当两点相同时
		//2. 当两点平行于y轴
		//3. 当两点平行于x轴
		//4. DDS算法
		if (x1 == x2 && y1 == y2)
			DrawPixel(x1, y1, c);
		else if (x1 == x2)
		{
			int inc = y1 < y2 ? 1 : -1;
			for (int i = y1; i != y2; i += inc)
				DrawPixel(x1, i, c);
			DrawPixel(x2, y2, c);
		}
		else if (y1 == y2)
		{
			int inc = x1 < x2 ? 1 : -1;
			for (int i = x1; i != x2; i += inc)
				DrawPixel(i, y1, c);
			DrawPixel(x2, y2, c);
		}
		else
		{
			int rem = 0;
			int x, y;
			int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
			int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
			if (dx >= dy) {
				if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
				for (x = x1, y = y1; x <= x2; x++)
				{
					DrawPixel(x, y, c);
					rem += dy;
					if (rem >= dx)
					{
						rem -= dx;
						y += (y2 >= y1) ? 1 : -1;
						DrawPixel(x, y, c);
					}
				}
				DrawPixel(x2, y2, c);
			}
			else {
				if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
				for (x = x1, y = y1; y <= y2; y++) {
					DrawPixel(x, y, c);
					rem += dx;
					if (rem >= dy) {
						rem -= dy;
						x += (x2 >= x1) ? 1 : -1;
						DrawPixel(x, y, c);
					}
				}
				DrawPixel(x2, y2, c);
			}
		}
	}

	/*!
	* @function Render3D::Raster::DrawScanline
	*
	* @brief 绘制扫描线
	*
	* @return void
	*
	* @param Scanline &scanline
	*/
	void Raster::DrawScanline(Scanline scanline)
	{
		uint32_t *framebuffer = m_framebuffer[scanline.y];
		float *zbuffer = m_zbuffer[scanline.y];
		int x = scanline.x;
		int w = scanline.w;
		int width = m_width;
		int render_state = m_render_state;
		for (; w > 0; x++, w--)
		{
			if (x >= 0 && x < width)
			{
				float rhw = scanline.v.rhw;
				if (rhw >= zbuffer[x])
				{
					float w = 1.0f / rhw;
					zbuffer[x] = rhw;
					if (render_state & RENDER_STATE_COLOR)
					{
						float r = scanline.v.color.r * w;
						float g = scanline.v.color.g * w;
						float b = scanline.v.color.b * w;
						int R = (int)(r * 255.0f);
						int G = (int)(g * 255.0f);
						int B = (int)(b * 255.0f);
						R = CMID(R, 0, 255);
						G = CMID(G, 0, 255);
						B = CMID(B, 0, 255);
						framebuffer[x] = (R << 16) | (G << 8) | (B);
					}
					if (render_state & RENDER_STATE_TEXTURE)
					{
						float u = scanline.v.tc.u * w;
						float v = scanline.v.tc.v * w;
						uint32_t cc = ReadTexture(u, v);
						framebuffer[x] = cc;
					}
				}
			}
			scanline.v.Add(scanline.step);
			if (x >= width) break;
		}
	}

	/*!
	* @function Render3D::Raster::RenderTrap
	*
	* @brief 主渲染函数
	*
	* @return void
	*
	* @param Trapezoid &trap
	*/
	void Raster::RenderTrap(Trapezoid &trap)
	{
		int top = int(trap.top + 0.5f);
		int bottom = int(trap.bottom + 0.5f);
		Scanline scanline;

		for (int y = top; y < bottom; y++)
		{
			if (y >= 0 && y < m_height)
			{
				//通过线性插值取得两条边纵坐标为y的点
				trap.InterpEdge(static_cast<float>(y)+ 0.5f);
				//根据左右两边的端点初始化扫描线
				trap.InitScanline(scanline, y);
				//根据扫描线画图
				DrawScanline(scanline);
			}
			else
				break;
		}
	}

	/*!
	* @function Render3D::Raster::DrawPrimitive
	*
	* @brief 根据render_state绘制原始三角形
	*
	* @return void
	*
	* @param const Vertex v1
	* @param const Vertex v2
	* @param const Vertex v3
	*/
	void Raster::DrawPrimitive(const Vertex v1, const Vertex v2, const Vertex v3)
	{
		Point p1, p2, p3, c1, c2, c3;
		//点坐标变换到CVV中
		m_transform.Apply(c1, v1.pos);
		m_transform.Apply(c2, v2.pos);
		m_transform.Apply(c3, v3.pos);

		//CVV裁剪
		if (Transform::CheckCVV(c1) != 0) return;
		if (Transform::CheckCVV(c2) != 0) return;
		if (Transform::CheckCVV(c3) != 0) return;

		//归一化到NDC  透视除法
		m_transform.Homogenize(p1, c1);
		m_transform.Homogenize(p2, c2);
		m_transform.Homogenize(p3, c3);

		int render_state = m_render_state;
		Trapezoids traps;
		//纹理和颜色模式
		if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR))
		{
			Vertex t1 = v1, t2 = v2, t3 = v3;
			t1.pos = p1;
			t2.pos = p2;
			t3.pos = p3;
			t1.pos.W() = c1.W();
			t2.pos.W() = c2.W();
			t3.pos.W() = c3.W();

			//点的初始化
			t1.InitWithRhw();
			t2.InitWithRhw();
			t3.InitWithRhw();

			//图元转配
			traps.InitTriangle(t1, t2, t3);

			//扫描线的区域填充
			if (traps.count >= 1) RenderTrap(traps[0]);
			if (traps.count >= 2) RenderTrap(traps[1]);
		}

		//线框模式
		if (render_state & RENDER_STATE_WIREFRAME)
		{
			//画线
			DrawLine((int)p1.X(), (int)p1.Y(), (int)p2.X(), (int)p2.Y(), m_foreground);
			DrawLine((int)p1.X(), (int)p1.Y(), (int)p3.X(), (int)p3.Y(), m_foreground);
			DrawLine((int)p3.X(), (int)p3.Y(), (int)p2.X(), (int)p2.Y(), m_foreground);
		}
	}

	void Raster::SetTransformWorld(const Math3D::Matrix44 _world)
	{
		m_transform.SetWorld(_world);
	}

	void Raster::SetTransformView(const Math3D::Matrix44 _view)
	{
		m_transform.SetView(_view);
	}

	void Raster::SetTransformProjection(const Math3D::Matrix44 _projection)
	{
		m_transform.SetProjection(_projection);
	}

	void Raster::TransformUpdate()
	{
		m_transform.Update();
	}
}
