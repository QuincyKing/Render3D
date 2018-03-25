#include <windows.h>
#include "window/Window.h"
#include "render/Raster.h"
#include "render/Primitive.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include "math/MathUtil.h"
#include "base/Color.h"
#include "base/Material.h"
#include "base/Texture.h"
#include "base\Light\PointLight.h"
#include "base\Light\DirLight.h"
#include "base\Camera.h"
#include <stdio.h>
#include <vector>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2test.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

extern std::vector<Base3D::Material> materials;
extern Base3D::DirLight dirLight;
extern std::vector<Base3D::PointLight> pointLights;
extern std::vector<Base3D::Camera> cameras;

Render3D::Vertex mesh[8] =
{
	{ { 1, -1, 1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ { -1, -1, 1, 1 }, { 0, 0 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ { -1, 1, 1, 1 }, { 0, 0 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { 1, 1, 1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	{ { 1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	{ { -1, -1, -1, 1 }, { 0, 0 }, { 0.2f, 1.0f, 1.0f }, 1 },
	{ { -1, 1, -1, 1 }, { 0, 0 }, { 1.0f, 0.3f, 0.3f }, 1 },
	{ { 1, 1, -1, 1 }, { 0, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};

void DrawPlane(Render3D::Raster &raster, int a, int b, int c, int d) 
{
	Render3D::Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	raster.DrawPrimitive(p1, p2, p3);
	raster.DrawPrimitive(p3, p4, p1);
}

void DrawBox(Render3D::Raster &raster, float theta)
{
	Math3D::Matrix44 world;
	MatrixSetRotate(world, -1, -0.5, 1, theta);
	raster.SetTransformWorld( world );
	raster.TransformUpdate();
	DrawPlane(raster, 0, 1, 2, 3);
	DrawPlane(raster, 4, 5, 6, 7);
	DrawPlane(raster, 0, 4, 5, 1);
	DrawPlane(raster, 1, 5, 6, 2);
	DrawPlane(raster, 2, 6, 7, 3);
	DrawPlane(raster, 3, 7, 4, 0);
}

void CameraAtZero(Render3D::Raster &raster, float x, float y, float z) 
{
	Math3D::Matrix44 view;
	Render3D::Point eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	MatrixSetLookat(view, eye, at, up);
	raster.SetTransformView(view);
	raster.TransformUpdate();
}

void InitTexture(Render3D::Raster &raster)
{
	static uint32_t texture[256][256];
	for (size_t j = 0; j < 256; j++) 
	{
		for (size_t i = 0; i < 256; i++) 
		{
			int x = i / 32, y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x000000;
		}
	}
	raster.SetTexture(texture, 256 * 4, 256, 256);
}

int main(void)
{
	Render3D::Raster raster;
	Window3D::Window window;
	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int indicator = 0;
	int flag = 0;
	float alpha = 1;
	float pos = 3.5;

	TCHAR *title = _T("Render3d")
		_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (window.Init(800, 600, title) != 0)
		return -1;

	raster.Init(800, 600, window.GetFrameBuffer());
	CameraAtZero(raster, 3, 0, 0);

	InitTexture(raster);
	raster.SetRenderState(RENDER_STATE_TEXTURE);

	while (true)
	{
		window.MsgLoop();
		raster.Clear();
		CameraAtZero(raster, pos, 0, 0);

		if (Window3D::Window::IsKey(VK_UP))		 pos -= 0.1f;
		if (Window3D::Window::IsKey(VK_DOWN))	 pos += 0.1f;
		if (Window3D::Window::IsKey(VK_LEFT))	 alpha += 0.1f;
		if (Window3D::Window::IsKey(VK_RIGHT))   alpha -= 0.1f;

		if (Window3D::Window::IsKey(VK_SPACE))
		{
			if (flag == 0)
			{
				flag = 1;
				if (++indicator >= 3) indicator = 0;
				raster.SetRenderState(states[indicator]);
			}
		}
		else
		{
			flag = 0;
		}

		DrawBox(raster, alpha);
		window.Update();
		Sleep(1);
	}
	return 0;
}