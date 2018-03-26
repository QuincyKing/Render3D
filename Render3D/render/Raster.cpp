#include "Raster.h"
#include "../math/Matrix.h"

namespace Render3D
{
	void Raster::Init(Raster _raster)
	{
		renderState = RENDER_STATE_TEXTURE;
		background = 0x0c0c0c;
		foreground = 0;
		
		Matrix44 identity;
		identity.MakeIdentity();
		transform.SetProjection(identity);
		transform.SetView(identity);
		transform.SetModel(identity);
	}

	void Raster::SetBackground(uint32_t _background)
	{
		background = _background;
	}

	void Raster::SetForeground(uint32_t _foreground)
	{
		foreground = _foreground;
	}

	void Raster::SetFrameBuffer(uint32_t *_framebuffer)
	{
		frameBuffer = _framebuffer;
	}

	void Raster::SetZBuffer(float *_zbuffer)
	{
		zBuffer = _zbuffer;
	}

	void Raster::SetShadowBuffer(float *_shadowbuffer)
	{
		shadowBuffer = _shadowbuffer;
	}

	void Raster::SetCamera(Base3D::Camera _camera)
	{
		camera = _camera;
		transform.SetView(_camera.viewMatrix);
		transform.SetViewR(_camera.viewMatrixR);
		transform.SetProjection(_camera.projectionMatrix);
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
		if (frameBuffer != NULL)
			free(frameBuffer);
		frameBuffer = NULL;
		zBuffer = NULL;
		shadowBuffer = NULL;
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
		if (frameBuffer != NULL) 
		{
			for (int y = 0; y < camera.height; y++)
				for (int x = 0; x < camera.width; x++)
					frameBuffer[y * camera.width + x] = background;
		}

		if (zBuffer != NULL)
			memset(zBuffer, 0, camera.width * camera.height * sizeof(float));
		if (shadowBuffer != NULL) {
			for (int y = 0; y < camera.height; y++)
				for (int x = 0; x < camera.width; x++)
					shadowBuffer[y * camera.width + x] = 1.0f;
		}
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
		if (x >= 0 && x < camera.width && y >= 0 && y < camera.height) 
		{
			frameBuffer[y * camera.width + x] = color;
		}
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
		int dx = x2 - x1;
		int dy = y2 - y1;
		int ux = ((dx > 0) << 1) - 1;
		int uy = ((dy > 0) << 1) - 1;
		int x = x1, y = y1, eps;

		eps = 0; dx = abs(dx); dy = abs(dy);
		if (dx > dy)
		{
			for (x = x1; x != x2 + ux; x += ux)
			{
				DrawPixel(x, y, c);
				eps += dy;
				if ((eps << 1) >= dx) {
					y += uy;
					eps -= dx;
				}
			}
		}
		else {
			for (y = y1; y != y2 + uy; y += uy)
			{
				DrawPixel(x, y, c);
				eps += dx;
				if ((eps << 1) >= dy) {
					x += ux;
					eps -= dy;
				}
			}
		}
	}

	void Raster::DrawScanline(Scanline *scanline, Point *points, v2f *vfs) 
	{
		int y = scanline->y;
		int x = scanline->x;
		int width = device->camera->width;
		int render_state = device->render_state;
		int count = scanline->w;
		for (; count > 0 && x < width; x++, count--) {
			if (x >= 0) {
				if (device->shadowbuffer != NULL) {
					float z = scanline->v.pos.z;
					if (z <= device->shadowbuffer[y*width + x]) {
						device->shadowbuffer[y*width + x] = z;
					}
				}

				float rhw = scanline->v.pos.w;
				if (device->zbuffer == NULL || rhw >= device->zbuffer[y*width + x]) {
					if (device->zbuffer != NULL)
						device->zbuffer[y*width + x] = rhw;

					if (device->framebuffer != NULL) {
						color_t color = { 0.0f, 0.0f, 0.0f, 1.0f };
						v2f vf;
						float w = 1.0f / scanline->v.pos.w;
						point_t barycenter = { 0.0f, 0.0f, 0.0f, 1.0f };
						point_t interpos = scanline->v.pos;
						transform_homogenize_reverse(&interpos, &interpos, w, device->camera->width, device->camera->height);
						computeBarycentricCoords3d(&barycenter, &points[0], &points[1], &points[2], &interpos);


						v2f_interpolating(&vf, &vfs[0], &vfs[1], &vfs[2], barycenter.x, barycenter.y, barycenter.z);
						vf.pos.w = w;
						vector_normalize(&vf.normal);

						frag_shader(device, &vf, &color);

						float a = 1.0f;
						float r = 0.0f;
						float g = 0.0f;
						float b = 0.0f;

						if (render_state & RENDER_STATE_COLOR) {
							a = vf.color.a;
							r = vf.color.r;
							g = vf.color.g;
							b = vf.color.b;
						}
						if (render_state & RENDER_STATE_TEXTURE) {
							a = color.a;
							r = color.r;
							g = color.g;
							b = color.b;
						}

						int A = CMID((int)(a * 255.0f), 0, 255);
						int R = CMID((int)(r * 255.0f), 0, 255);
						int G = CMID((int)(g * 255.0f), 0, 255);
						int B = CMID((int)(b * 255.0f), 0, 255);

						device->framebuffer[y*width + x] = (R << 16) | (G << 8) | B;
					}
				}
			}
			vertex_add(&scanline->v, &scanline->step);
		}
	}
}
