#include "Raster.h"
#include "../math/Matrix.h"

namespace Render3D
{
	void Raster::Init()
	{
		renderState = RENDER_STATE_TEXTURE;
		background = 0x0c0c0c;
		foreground = 0;
		
		Matrix44 identity;
		identity.MakeIdentity();
		transform.projection = identity;
		transform.view = identity;
		transform.model = identity;
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
		transform.view = _camera.viewMatrix;
		transform.viewR = _camera.viewMatrixR;
		transform.projection = _camera.projectionMatrix;
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
				float bias = 0.015f * (1.0f - dot);
				if (bias < 0.002f) bias = 0.001f;
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
							shadow += tempPos.Z() - bias > pcfDepth ? 1.0f : 0.0f;
						}
					}
					shadow /= 9.0f;

					Base3D::Color temp = { 0.3f, 0.3f, 0.3f, 0.3f };
					temp = temp * shadow;
					color = color - temp;
				}
			}
		}


		uint32_t i = 0;
		for (; i < pointLights.size(); i++)
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

	static bool ComputeBarycentricCoords3d(Render3D::Point &res, const Render3D::Point &p0, const Render3D::Point &p1, const Render3D::Point &p2, const Render3D::Point &p)
	{
		Vector4 d1, d2, n;
		d1 = p1 - p0;
		d2 = p2 - p1;
		n = Cross(d1, d2);
		float u1, u2, u3, u4;
		float v1, v2, v3, v4;
		if ((fabs(n.X()) >= fabs(n.Y())) && (fabs(n.X()) >= fabs(n.Z())))
		{
			u1 = p0.Y() - p2.Y();
			u2 = p1.Y() - p2.Y();
			u3 = p.Y() - p0.Y();
			u4 = p.Y() - p2.Y();
			v1 = p0.Z() - p2.Z();
			v2 = p1.Z() - p2.Z();
			v3 = p.Z() - p0.Z();
			v4 = p.Z() - p2.Z();
		}
		else if (fabs(n.Y()) >= fabs(n.Z()))
		{
			u1 = p0.Z() - p2.Z();
			u2 = p1.Z() - p2.Z();
			u3 = p.Z() - p0.Z();
			u4 = p.Z() - p2.Z();
			v1 = p0.X() - p2.X();
			v2 = p1.X() - p2.X();
			v3 = p.X() - p0.X();
			v4 = p.X() - p2.X();
		}
		else
		{
			u1 = p0.X() - p2.X();
			u2 = p1.X() - p2.X();
			u3 = p.X() - p0.X();
			u4 = p.X() - p2.X();
			v1 = p0.Y() - p2.Y();
			v2 = p1.Y() - p2.Y();
			v3 = p.Y() - p0.Y();
			v4 = p.Y() - p2.Y();
		}

		float denom = v1 * u2 - v2 * u1;
		if (fabsf(denom) < 1e-6)
		{
			return false;
		}
		float oneOverDenom = 1.0f / denom;
		res.X() = (v4 * u2 - v2 * u4) * oneOverDenom;
		res.Y() = (v1 * u3 - v3 * u1) * oneOverDenom;
		res.Z() = 1.0f - res.X() - res.Y();
		return true;
	}

	void Raster::DrawScanline(Scanline &scanline, Point *points, Base3D::v2f *vfs) 
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
						ComputeBarycentricCoords3d(barycenter, points[0], points[1], points[2], interpos);

						V2fInterpolating(vf, vfs[0], vfs[1], vfs[2], barycenter.X(), barycenter.Y(), barycenter.Z());
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

	void Raster::RenderTrap(Trapezoid &trap, Point *points, Base3D::v2f *v2fs)
	{
		Scanline scanline;
		int j, top, bottom;
		top = (int)(trap.top + 0.5f);
		bottom = (int)(trap.bottom + 0.5f);
		for (j = top; j < bottom; j++)
		{
			if (j >= 0 && j < camera.height) 
			{
				trap.InterpEdge((float)j + 0.5f);
				trap.InitScanline(scanline, j);
				DrawScanline(scanline, points, v2fs);
			}
			if (j >= camera.height)
				break;
		}
	}

	void CalculateTangentAndBinormal(Vector4 &tangent, Vector4 &binormal, const Vector4 &position1, const Vector4 &position2, const Vector4 &position3,
		float u1, float v1, float u2, float v2, float u3, float v3)
	{
		//side0 is the vector along one side of the triangle of vertices passed in,
		//and side1 is the vector along another side. Taking the cross product of these returns the normal.
		Vector4 side0(0.0f, 0.0f, 0.0f, 1.0f);
		side0 = position1 - position2;
		Vector4 side1(0.0f, 0.0f, 0.0f, 1.0f);
		side1 = position3 - position1;

		//Calculate face normal
		Vector4 normal(0.0f, 0.0f, 0.0f, 0.0f);
		normal = Cross(side1, side0);
		Normalize(normal);
		//Now we use a formula to calculate the tangent.
		float deltaV0 = v1 - v2;
		float deltaV1 = v3 - v1;
		tangent = side0;
		tangent = tangent *  deltaV1;
		Vector4 temp = side1;
		temp = temp * deltaV0;
		tangent = tangent - temp;
		Normalize(tangent);
		//Calculate binormal
		float deltaU0 = u1 - u2;
		float deltaU1 = u3 - u1;
		binormal = side0;
		binormal = binormal * deltaU1;
		temp = side1;
		temp = temp * deltaU0;
		binormal = binormal - temp;
		Normalize(binormal);
		//Now, we take the cross product of the tangents to get a vector which
		//should point in the same direction as our normal calculated above.
		//If it points in the opposite direction (the dot product between the normals is less than zero),
		//then we need to reverse the s and t tangents.
		//This is because the triangle has been mirrored when going from tangent space to object space.
		//reverse tangents if necessary
		Vector4 tangentCross;
		tangentCross = (tangent, binormal);
		if (Dot(tangentCross, normal) < 0.0f)
		{
			tangent.W() = -1;
		}
	}

	void Raster::DrawPrimitive(Vertex &t1, Vertex &t2, Vertex &t3)
	{
		Vertex vertice[3] = { t1, t2, t3 };
		Point points[3];

		// 正规矩阵
		Matrix44 nm(transform.model);
		nm = MatrixInverse(nm);
		nm = MatrixTranspose(nm);

		Base3D::a2v a2vs[3];
		Base3D::v2f v2fs[3];
		for (int i = 0; i < 3; i++)
		{
			Vertex *vertex = &vertice[i];
			Base3D::a2v *av = &a2vs[i];

			av->pos = vertex->pos; // 世界空间的pos
			int a = 0, b = 0;
			if (i == 0) a = 1, b = 2;
			if (i == 1) a = 0, b = 2;
			if (i == 2) a = 0, b = 1;
			CalculateTangentAndBinormal(av->tangent, av->binormal, vertex->pos, vertice[a].pos, vertice[b].pos, vertex->tc.u, vertex->tc.v, vertice[a].tc.u, vertice[a].tc.v, vertice[b].tc.u, vertice[b].tc.v);

			av->tangent = av->tangent * transform.model;
			av->binormal = Cross(av->normal, av->tangent);
			av->binormal = av->binormal * av->tangent.W();
			vertex->pos = vertex->pos * transform.vp;
			points[i] = vertex->pos; // 透视空间的pos

			vertex->normal = vertex->normal * nm; // 法向量乘正规矩阵
			Normalize(vertex->normal);
			av->normal = vertex->normal; // 世界空间的normal
			av->color = vertex->color;
			av->texcoord.u = vertex->tc.u;
			av->texcoord.v = vertex->tc.v;

			VertShader(*av, v2fs[i]); // 顶点着色器

			Homogenize(vertex->pos, vertex->pos, camera.width, camera.height);
		}

		// 背面剔除
		if (cull > 0) 
		{
			Vector4 t21, t32;
			t21 = t2.pos - t1.pos;
			t32 = t3.pos - t2.pos;
			if (cull == 1) 
			{
				if (t21.X() * t32.Y() - t32.X() * t21.Y() <= 0)    // 计算叉积
					return;
			}
			else if (cull == 2) 
			{
				if (t21.X() * t32.Y() - t32.X() * t21.Y() > 0)     // 计算叉积
					return;
			}
		}

		if (renderState & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) 
		{
			Trapezoids traps;
			traps.InitTriangle(t1, t2, t3);
			if (traps.count >= 1) RenderTrap(traps[0], points, v2fs);
			if (traps.count >= 2) RenderTrap(traps[1], points, v2fs);
		}

		if ((renderState & RENDER_STATE_WIREFRAME) && frameBuffer != NULL) 
		{
			DrawLine((int)t1.pos.X(), (int)t1.pos.Y(), (int)t2.pos.X(), (int)t2.pos.Y(), foreground);
			DrawLine((int)t1.pos.X(), (int)t1.pos.Y(), (int)t3.pos.X(), (int)t3.pos.Y(), foreground);
			DrawLine((int)t3.pos.X(), (int)t3.pos.Y(), (int)t2.pos.X(), (int)t2.pos.Y(), foreground);
		}
	}

	void Raster::ClipPolys(Vertex &v1, Vertex &v2, Vertex &v3, bool world) 
	{
		#define CLIP_CODE_GZ    0x0001
		#define CLIP_CODE_LZ    0x0002
		#define CLIP_CODE_IZ    0x0004

		#define CLIP_CODE_GX    0x0001
		#define CLIP_CODE_LX    0x0002
		#define CLIP_CODE_IX    0x0004

		#define CLIP_CODE_GY    0x0001
		#define CLIP_CODE_LY    0x0002
		#define CLIP_CODE_IY    0x0004

		#define CLIP_CODE_NULL  0x0000

		int vertex_ccodes[3];
		int num_verts_in = 0;

		float z_factor_x, z_factor_y, z_factor, z_test;

		float xi, yi, x01i, y01i, x02i, y02i, t1, t2, ui, vi, u01i, v01i, u02i, v02i;

		bool cliped = false;

		Vector4 v;

		Vertex p1 = v1, p2 = v2, p3 = v3;

		if (world == false)
		{
			p1.pos = p1.pos * transform.mv;
			p2.pos = p2.pos * transform.mv;
			p3.pos = p3.pos * transform.mv;
		}
		else
		{
			p1.pos = p1.pos * transform.view;
			p2.pos = p2.pos * transform.view;
			p3.pos = p3.pos * transform.view;
		}


		z_factor_y = tan(camera.fovy*0.5f);
		z_factor_x = z_factor_y / camera.aspect;
		z_factor = z_factor_x;
		z_test = z_factor * p1.pos.Z();

		if (p1.pos.X() > z_test)
			vertex_ccodes[0] = CLIP_CODE_GX;
		else if (p1.pos.X() < -z_test)
			vertex_ccodes[0] = CLIP_CODE_LX;
		else
			vertex_ccodes[0] = CLIP_CODE_IX;

		z_test = z_factor * p2.pos.Z();

		if (p2.pos.X() > z_test)
			vertex_ccodes[1] = CLIP_CODE_GX;
		else if (p2.pos.X() < -z_test)
			vertex_ccodes[1] = CLIP_CODE_LX;
		else
			vertex_ccodes[1] = CLIP_CODE_IX;

		z_test = z_factor * p3.pos.Z();

		if (p3.pos.X() > z_test)
			vertex_ccodes[2] = CLIP_CODE_GX;
		else if (p3.pos.X() < -z_test)
			vertex_ccodes[2] = CLIP_CODE_LX;
		else
			vertex_ccodes[2] = CLIP_CODE_IX;

		if (((vertex_ccodes[0] == CLIP_CODE_GX) && (vertex_ccodes[1] == CLIP_CODE_GX) && (vertex_ccodes[2] == CLIP_CODE_GX))
			|| ((vertex_ccodes[0] == CLIP_CODE_LX) && (vertex_ccodes[1] == CLIP_CODE_LX) && (vertex_ccodes[2] == CLIP_CODE_LX)))
		{
			return;
		}

		z_factor = z_factor_y;
		z_test = z_factor * p1.pos.Z();

		if (p1.pos.Y() > z_test)
			vertex_ccodes[0] = CLIP_CODE_GY;
		else if (p1.pos.Y() < -z_test)
			vertex_ccodes[0] = CLIP_CODE_LY;
		else
			vertex_ccodes[0] = CLIP_CODE_IY;

		z_test = z_factor * p2.pos.Z();

		if (p2.pos.Y() > z_test)
			vertex_ccodes[1] = CLIP_CODE_GY;
		else if (p2.pos.Y() < -z_test)
			vertex_ccodes[1] = CLIP_CODE_LY;
		else
			vertex_ccodes[1] = CLIP_CODE_IY;

		z_test = z_factor * p3.pos.Z();

		if (p3.pos.Y() > z_test)
			vertex_ccodes[2] = CLIP_CODE_GY;
		else if (p3.pos.Y() < -z_test)
			vertex_ccodes[2] = CLIP_CODE_LY;
		else
			vertex_ccodes[2] = CLIP_CODE_IY;

		if (((vertex_ccodes[0] == CLIP_CODE_GY) && (vertex_ccodes[1] == CLIP_CODE_GY) && (vertex_ccodes[2] == CLIP_CODE_GY))
			|| ((vertex_ccodes[0] == CLIP_CODE_LY) && (vertex_ccodes[1] == CLIP_CODE_LY) && (vertex_ccodes[2] == CLIP_CODE_LY)))
		{
			return;
		}

		// 是否完全在远裁剪面或近裁剪面外侧
		if (p1.pos.Z() > camera.zFar)
			vertex_ccodes[0] = CLIP_CODE_GZ;
		else if (p1.pos.Z() < camera.zNear)
			vertex_ccodes[0] = CLIP_CODE_LZ;
		else 
		{
			vertex_ccodes[0] = CLIP_CODE_IZ;
			num_verts_in++;
		}

		if (p2.pos.Z() > camera.zFar)
			vertex_ccodes[1] = CLIP_CODE_GZ;
		else if (p2.pos.Z() < camera.zNear)
			vertex_ccodes[1] = CLIP_CODE_LZ;
		else 
		{
			vertex_ccodes[1] = CLIP_CODE_IZ;
			num_verts_in++;
		}


		if (p3.pos.Z() > camera.zFar)
			vertex_ccodes[2] = CLIP_CODE_GZ;
		else if (p3.pos.Z() < camera.zNear)
			vertex_ccodes[2] = CLIP_CODE_LZ;
		else 
		{
			vertex_ccodes[2] = CLIP_CODE_IZ;
			num_verts_in++;
		}


		if (((vertex_ccodes[0] == CLIP_CODE_GZ) && (vertex_ccodes[1] == CLIP_CODE_GZ) && (vertex_ccodes[2] == CLIP_CODE_GZ))
			|| ((vertex_ccodes[0] == CLIP_CODE_LZ) && (vertex_ccodes[1] == CLIP_CODE_LZ) && (vertex_ccodes[2] == CLIP_CODE_LZ)))
		{
			return;
		}

		// 判断是否有顶点在近裁剪面外侧
		if (((vertex_ccodes[0] | vertex_ccodes[1] | vertex_ccodes[2]) & CLIP_CODE_LZ))
		{
			Vertex temp;
			//num_verts_in = 0;
			// 三角形有1个顶点在近裁剪面内侧，2个顶点在外侧
			// 三角形有2个顶点在近裁剪面内侧，1个顶点在外侧
			if (num_verts_in == 1)
			{
				if (vertex_ccodes[0] == CLIP_CODE_IZ) 
				{
				}
				else if (vertex_ccodes[1] == CLIP_CODE_IZ) 
				{
					temp = p1;
					p1 = p2;
					p2 = p3;
					p3 = temp;
				}
				else
				{
					temp = p1;
					p1 = p3;
					p3 = p2;
					p2 = temp;
				}
				// 对每条边进行裁剪
				// 创建参数化方程p = v0 + v01 * t
				v = p2.pos- p1.pos;
				t1 = (camera.zNear - p1.pos.Z()) / v.Z();

				xi = p1.pos.X() + v.X() * t1;
				yi = p1.pos.Y() + v.Y() * t1;

				// 用交点覆盖原来的顶点
				p2.pos.X() = xi;
				p2.pos.Y() = yi;
				p2.pos.Z() = camera.zNear;

				// 对三角形边v0->v2进行裁剪
				v = p3.pos - p1.pos;
				t2 = (camera.zNear - p1.pos.Z()) / v.Z();

				xi = p1.pos.X() + v.X() * t2;
				yi = p1.pos.Y() + v.Y() * t2;

				// 用交点覆盖原来的顶点
				p3.pos.X() = xi;
				p3.pos.Y() = yi;
				p3.pos.Z() = camera.zNear;

				// 对纹理进行裁剪
				ui = p1.tc.u + (p2.tc.u - p1.tc.u) * t1;
				vi = p1.tc.v + (p2.tc.v - p1.tc.v) * t1;

				p2.tc.u = ui;
				p2.tc.v = vi;

				ui = p1.tc.u + (p3.tc.u - p1.tc.u) * t2;
				vi = p1.tc.v + (p3.tc.v - p1.tc.v) * t2;

				p3.tc.u = ui;
				p3.tc.v = vi;

				cliped = true;
			}
			else if (num_verts_in == 2)
			{
				// 外侧的点
				if (vertex_ccodes[0] == CLIP_CODE_LZ) 
				{
				}
				else if (vertex_ccodes[1] == CLIP_CODE_LZ)
				{
					temp = p1;
					p1 = p2;
					p2 = p3;
					p3 = temp;
				}
				else 
				{
					temp = p1;
					p1 = p3;
					p3 = p2;
					p2 = temp;
				}

				Vertex np1 = p1, np2 = p2, np3 = p3;
				// 对每条边进行裁剪
				// 创建参数化方程p = v0 + v01 * t
				v = p2.pos - p1.pos;
				t1 = (camera.zNear - p1.pos.Z()) / v.Z();

				x01i = p1.pos.X() + v.X() * t1;
				y01i = p1.pos.Y() + v.Y() * t1;

				// 对三角形边v0->v2进行裁剪
				v = p3.pos - p1.pos;
				t2 = (camera.zNear - p1.pos.Z()) / v.Z();

				x02i = p1.pos.X() + v.X() * t2;
				y02i = p1.pos.Y() + v.Y() * t2;

				// 用交点覆盖原来的顶点
				p1.pos.X() = x01i;
				p1.pos.Y() = y01i;
				p1.pos.Z() = camera.zNear;

				np2.pos.X() = x01i;
				np2.pos.Y() = y01i;
				np2.pos.Z() = camera.zNear;

				np1.pos.X() = x02i;
				np1.pos.Y() = y02i;
				np1.pos.Z() = camera.zNear;

				// 对纹理进行裁剪
				u01i = p1.tc.u + (p2.tc.u - p1.tc.u) * t1;
				v01i = p1.tc.v + (p2.tc.v - p1.tc.v) * t1;

				u02i = p1.tc.u + (p3.tc.u - p1.tc.u) * t2;
				v02i = p1.tc.v + (p3.tc.v - p1.tc.v) * t2;

				p1.tc.u = u01i;
				p1.tc.v = v01i;

				np1.tc.u = u02i;
				np1.tc.v = v02i;
				np2.tc.u = u01i;
				np2.tc.v = v01i;

				np1.pos = np1.pos * transform.viewR;
				np2.pos = np2.pos * transform.viewR;
				np3.pos = np3.pos * transform.viewR;
				DrawPrimitive(np1, np2, np3);

				cliped = true;
			}
	}
	p1.pos = p1.pos * transform.viewR;
	p2.pos = p2.pos * transform.viewR;
	p3.pos = p3.pos * transform.viewR;
	DrawPrimitive(p1, p2, p3);
}

}
