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

	void Raster::VertShader(Base3D::a2v &av, Base3D::v2f &vf)
	{
		vf.pos = av.pos;
		vf.normal = av.normal;
		vf.color = av.color;
		vf.texcoord = av.texcoord;

		vf.storage0 = { av.tangent.X(), av.binormal.X(), av.normal.X() };
		vf.storage1 = { av.tangent.Y(), av.binormal.Y(), av.normal.Y() };
		vf.storage2 = { av.tangent.Z(), av.binormal.Z(), av.normal.Z() };
	}

	Base3D::DirLight dirLight;
	std::vector<Base3D::Texture> textures;
	std::vector<Base3D::Camera> cameras;
	float *pShadowBuffer;
	std::vector<Base3D::PointLight> pointLights;

	void Raster::FragShader(Base3D::v2f &vf, Base3D::Color &color)
	{
		Base3D::Material material = this->material;
		Math3D::Vector4 viewdir, viewPos = camera.position;
		viewdir = viewPos - vf.pos;
		Normalize(viewdir);
		Base3D::Texcoord tex = vf.texcoord;
		Math3D::Vector4 normal = vf.normal;

		Math3D::Vector4 lightDir = dirLight.direction;
		VectorInverse(lightDir);
		Normalize(lightDir);

		float diff = fmaxf(Dot(normal, lightDir), 0.0f);
		lightDir = dirLight.direction;
		Normalize(lightDir);
		Math3D::Vector4 vec;
		Reflect(vec, lightDir, normal);
		float shininess = material.shininess * (material.specularHighlightTexId == -1 ? 1 : textures[material.specularHighlightTexId].TextureValueRead(tex.u, tex.v));
		float spec = powf(fmaxf(Dot(viewdir, vec), 0.0f), shininess);

		Base3D::Color temp = { 0.0f, 0.0f, 0.0f, 1.0f };
		Base3D::Color temp2 = material.ambientTexId == -1 ? material.ambient : textures[material.ambientTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
		ColorProduct(temp, dirLight.abmient, temp2);
		color += temp;
		temp2 = material.diffuseTexId == -1 ? material.diffuse : textures[material.diffuseTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
		ColorProduct(temp, dirLight.diffuse, temp2);
		temp = temp * diff;
		color += temp;
		temp2 = material.specularTexId == -1 ? material.specular : textures[material.specularTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
		ColorProduct(temp, dirLight.specular, temp2);
		temp = temp * spec;
		color += temp;

		// 计算阴影
		if (dirLight.shadow)
		{
			// fragPos -> 灯的坐标系 -> 灯的透视矩阵 -> 求得z坐标比较
			Point tempPos = vf.pos;
			tempPos.W() = 1.0f;
			Base3D::Camera *camera = &cameras[0];
			tempPos = tempPos * camera->viewMatrix;
			tempPos = tempPos * camera->projectionMatrix;
			Homogenize(tempPos, tempPos, camera->width, camera->height);
			int y = (int)(tempPos.Y() + 0.5);
			int x = (int)(tempPos.X() + 0.5);

			Math3D::Vector4 tempNormal = vf.normal;
			tempNormal = tempNormal * camera->viewMatrix;
			VectorInverse(tempNormal);
			float dot = Dot(tempNormal, camera->front);
			if (dot > 0)
			{
				float bias = 0.015 * (1.0 - dot);
				if (bias < 0.002f) bias = 0.001;
				if (y >= 0 && x >= 0 && y < camera->height && x < camera->width)
				{
					float shadow = 0.0;
					for (int i = -1; i <= 1; ++i)
					{
						for (int j = -1; j <= 1; ++j)
						{
							if (y + j < 0 || y + j >= camera->height || x + i < 0 || x + i >= camera->width)
								continue;
							float pcfDepth = pShadowBuffer[(y + j) * camera->width + (x + i)];
							shadow += tempPos.Z() - bias > pcfDepth ? 1.0 : 0.0;
						}
					}
					shadow /= 9.0;

					Base3D::Color temp = { 0.3f, 0.3f, 0.3f, 0.3f };
					temp = temp * shadow;
					color = color - temp;
				}
			}
		}


		int i = 0;
		for (i = 0; i < pointLights.size(); i++)
		{
			temp = Base3D::Color(0.0f, 0.0f, 0.0f, 1.0f);
			Base3D::PointLight pointlight = pointLights[i];

			Math3D::Vector4 lightDir = { 0.0f, 0.0f, 0.0f, 0.0f };
			lightDir = pointlight.position - vf.pos;
			float distance = Length(lightDir);
			Normalize(lightDir);
			float diff = fmaxf(Dot(normal, lightDir), 0.0f);
			Math3D::Vector4 vec;
			VectorInverse(lightDir);
			Reflect(vec, lightDir, normal);
			float shininess = material.shininess * (material.specularHighlightTexId == -1 ? 1 : textures[material.specularHighlightTexId].TextureValueRead(tex.u, tex.v));
			float spec = powf(fmaxf(Dot(viewdir, vec), 0.0f), shininess);
			float num = pointlight.constant + pointlight.linear * distance + pointlight.quadratic * (distance * distance);
			float attenuation = 0;
			if (num != 0)
				attenuation = 1.0f / num;

			Base3D::Color c = Base3D::Color(0.0f, 0.0f, 0.0f, 1.0f);
			Base3D::Color c2 = material.ambientTexId == -1 ? material.ambient : textures[material.ambientTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
			ColorProduct(c, pointlight.abmient, c2);
			c = c * attenuation;
			temp = temp + c;
			c2 = material.diffuseTexId == -1 ? material.diffuse : textures[material.diffuseTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
			ColorProduct(c, pointlight.diffuse, c2);
			c = c * (diff * attenuation);
			temp = temp + c;
			c2 = material.specularTexId == -1 ? material.specular : textures[material.specularTexId].TextureRead(tex.u, tex.v, vf.pos.W(), 15);
			ColorProduct(c, pointlight.specular, c2);
			c = c * (spec * attenuation);
			temp = temp + c;

			color = color + temp;
		}
	}

	void Raster::DrawScanline(Scanline &scanline, Point &points, Base3D::v2f &vfs) 
	{
		int y = scanline.y;
		int x = scanline.x;
		int width = camera.width;
		int render_state = renderState;
		int count = scanline.w;
		for (; count > 0 && x < width; x++, count--)
		{
			if (x >= 0)
			{
				if (shadowBuffer != NULL)
				{
					float z = scanline.v.pos.Z();
					if (z <= shadowBuffer[y*width + x]) 
					{
						shadowBuffer[y*width + x] = z;
					}
				}

				float rhw = scanline.v.pos.W();
				if (zBuffer == NULL || rhw >= zBuffer[y*width + x])
				{
					if (zBuffer != NULL)
						zBuffer[y*width + x] = rhw;

					if (frameBuffer != NULL) 
					{
						Base3D::Color color = Base3D::Color(0.0f, 0.0f, 0.0f, 1.0f);
						Base3D::v2f vf;
						float w = 1.0f / scanline.v.pos.W();
						Point barycenter(0.0f, 0.0f, 0.0f, 1.0f);
						Point interpos = scanline.v.pos;
					    HomogenizeReverse(interpos, interpos, w, camera.width, camera.height);
						ComputeBarycentricCoords3d(&barycenter, &points[0], &points[1], &points[2], &interpos);


						V2fInterpolating(vf, vfs[0], vfs[1], vfs[2], barycenter.x, barycenter.y, barycenter.z);
						vf.pos.W() = w;
						Normalize(vf.normal);

						FragShader(vf, color);

						float a = 1.0f;
						float r = 0.0f;
						float g = 0.0f;
						float b = 0.0f;

						if (render_state & RENDER_STATE_COLOR)
						{
							a = vf.color.a;
							r = vf.color.r;
							g = vf.color.g;
							b = vf.color.b;
						}
						if (render_state & RENDER_STATE_TEXTURE)
						{
							a = color.a;
							r = color.r;
							g = color.g;
							b = color.b;
						}

						int A = Clamp((int)(a * 255.0f), 0, 255);
						int R = Clamp((int)(r * 255.0f), 0, 255);
						int G = Clamp((int)(g * 255.0f), 0, 255);
						int B = Clamp((int)(b * 255.0f), 0, 255);

						frameBuffer[y*width + x] = (R << 16) | (G << 8) | B;
					}
				}
			}
			scanline.v.Add(scanline.step);
		}
	}
}
