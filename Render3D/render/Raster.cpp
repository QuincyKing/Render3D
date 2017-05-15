#include "Raster.h"

namespace Render3D
{
	/*!
	* ����ȫ�� Render3D::Raster::Init
	*
	* @brief �豸��ʼ����fbΪ�ⲿ֡���棬�� NULL �������ⲿ֡���棨ÿ�� 4�ֽڶ��룩
	*
	* @return void
	*
	* @param int width ��Ļ���
	* @param int height ��Ļ�߶�
	* @param void * fb ֡����
	*/
	void Raster::Init(int width, int height, void *fb)
	{
		//1. framebuffer Ϊ height * sizeof(void *)
		//2. zbuffer Ϊheight * sizeof(void *)
		//3. texture Ϊ1024 * sizeof(void *)
		//4. framebuf�� Ϊ 4 * width * height
		//5. zbuf�� Ϊ4 * width * height
		//6. texure Ϊ64
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
		//��̬����framebuffer��zbuffer�Ķ�ά����
		for (int j = 0; j < height; j++)
		{
			m_framebuffer[j] = (uint32_t *)(framebuf + j * width * 4);
			m_zbuffer[j] = (float *)(zbuf + j * width * 4);
		}
		m_texture[0] = (uint32_t *)ptr;
		m_texture[1] = (uint32_t *)(ptr + 64);
		memset(m_texture[0], 0, 64);
		//��ʼ�����ξ���
		m_transform.Init(width, height);
	}

	/*!
	* ����ȫ�� Render3D::Raster::Destroy
	*
	* @brief ɾ���豸
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
	* ����ȫ�� Render3D::Raster::SetTexture
	*
	* @brief ���õ�ǰ����
	*
	* @return void
	*
	* @param void * bits ����
	* @param long pitch ����������
	* @param int w �����
	* @param int h ����
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
	* ����ȫ�� Render3D::Raster::Clear
	*
	* @brief ���framebuffer��zbuffer
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
	* @brief ����uv�����ȡ����
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
	* ����ȫ�� Render3D::Raster::DrawPixel
	*
	* @brief ����
	*
	* @return void
	*
	* @param int x ����x����
	* @param int y ����y����
	* @param uint32_t color ������ɫ
	*/
	void Raster::DrawPixel(int x, int y, uint32_t color)
	{
		if (x < m_width && y < m_height)
			m_framebuffer[y][x] = color;
	}

	/*!
	* ����ȫ�� Render3D::Raster::DrawLine
	*
	* @brief DDS�㷨�����߶�
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
		//1. ��������ͬʱ
		//2. ������ƽ����y��
		//3. ������ƽ����x��
		//4. DDS�㷨
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
	* @brief ����ɨ����
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
	* @brief ����Ⱦ����
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
				//ͨ�����Բ�ֵȡ��������������Ϊy�ĵ�
				trap.InterpEdge(static_cast<float>(y)+ 0.5f);
				//�����������ߵĶ˵��ʼ��ɨ����
				trap.InitScanline(scanline, y);
				//����ɨ���߻�ͼ
				DrawScanline(scanline);
			}
			else
				break;
		}
	}

	/*!
	* @function Render3D::Raster::DrawPrimitive
	*
	* @brief ����render_state����ԭʼ������
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
		//������任��CVV��
		m_transform.Apply(c1, v1.pos);
		m_transform.Apply(c2, v2.pos);
		m_transform.Apply(c3, v3.pos);

		//CVV�ü�
		if (Transform::CheckCVV(c1) != 0) return;
		if (Transform::CheckCVV(c2) != 0) return;
		if (Transform::CheckCVV(c3) != 0) return;

		//��һ����NDC  ͸�ӳ���
		m_transform.Homogenize(p1, c1);
		m_transform.Homogenize(p2, c2);
		m_transform.Homogenize(p3, c3);

		int render_state = m_render_state;
		Trapezoids traps;
		//�������ɫģʽ
		if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR))
		{
			Vertex t1 = v1, t2 = v2, t3 = v3;
			t1.pos = p1;
			t2.pos = p2;
			t3.pos = p3;
			t1.pos.W() = c1.W();
			t2.pos.W() = c2.W();
			t3.pos.W() = c3.W();

			//��ĳ�ʼ��
			t1.InitWithRhw();
			t2.InitWithRhw();
			t3.InitWithRhw();

			//ͼԪת��
			traps.InitTriangle(t1, t2, t3);

			//ɨ���ߵ��������
			if (traps.count >= 1) RenderTrap(traps[0]);
			if (traps.count >= 2) RenderTrap(traps[1]);
		}

		//�߿�ģʽ
		if (render_state & RENDER_STATE_WIREFRAME)
		{
			//����
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
