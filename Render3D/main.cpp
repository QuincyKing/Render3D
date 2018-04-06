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
#include "base\Object.h"
#include "base\Texture.h"
#include "SDL.h"
#include "utils\Utils.h"
#include <iostream>

#include <stdio.h>
#include <vector>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2test.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

std::vector<Base3D::Material> materials;
Base3D::DirLight dirLight;
std::vector<Base3D::PointLight> pointLights;
std::vector<Base3D::Camera> cameras;
std::vector<Base3D::Object> objs;
std::vector<Base3D::Texture> textures;
float *pShadowBuffer;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

const int REAL_WIDTH = 600;
const int REAL_HEIGHT = 600;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool sdl_init(int width, int height, const char *title)
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}

	return success;
}

void sdl_close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	//IMG_Quit();
	SDL_Quit();
}

Render3D::Vertex ground_mesh[6] = 
{
	// Positions                  // Texture Coords  //color           //rhw // Normals
	{ { -0.5f, 0.0f, -0.5f, 1.0f }, { 0.0f, 8.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.0f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.0f, 0.5f, 1.0f }, { 8.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.0f, 0.5f, 1.0f }, { 8.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.0f, -0.5f, 1.0f }, { 8.0f, 8.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.0f, -0.5f, 1.0f }, { 0.0f, 8.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } }
};

Render3D::Vertex box_mesh[36] = 
{
	// Positions                  // Texture Coords  //color           //rhw // Normals
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } },

	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } },

	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f, 0.0f } },

	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },

	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 0.0f, -1.0f, 0.0f, 0.0f } },

	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f }, { 0.2f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } }
};

void InitTexture()
{
	// 自建棋盘纹理
	int width = 256, height = 256;
	Base3D::Texture texture;
	uint32_t *bits = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
	int i, j;
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++) 
		{
			int x = i / 32, y = j / 32;
			bits[j*width + i] = ((x + y) & 1) ? 0xffffffff : 0xff3fbcef;
		}
	}
	texture.datasLen = 1;
	texture.datas = (uint32_t**)malloc(1 * sizeof(uint32_t*));
	texture.datas[0] = bits;
	texture.width = width;
	texture.height = height;
	texture.useMipmap = true;
	Utils3D::GenerateMipmaps(texture, 1.01f);

	// libpng读取外部纹理
	Utils3D::MakeTextureByPng("mabu", true);
	Utils3D::MakeTextureByPng("dimian", true);
}

void FreeTextures() 
{
	for (uint32_t i = 0; i < textures.size(); i++) 
	{
		Base3D::Texture *texture = &textures[i];
		for (uint32_t j = 0; j < texture->datasLen; j++)
		{
			uint32_t *data = texture->datas[j];
			free(data);
		}
		free(texture->datas);
		texture->datas = NULL;
	}
}

void DrawObject(Render3D::Raster &device, std::vector<Base3D::Object> &objects) 
{
	for (uint32_t i = 0; i < objects.size(); i++)
	{
		Base3D::Object *object = &objects[i];
		if (object->dirty == true) 
		{
			Math3D::MatrixSetScale(object->matrix, object->scale.X(), object->scale.Y(), object->scale.Z());
			Matrix44 r, t = object->matrix;
			MatrixSetRotate(r, object->axis.X(), object->axis.Y(), object->axis.Z(), object->theta);
			object->matrix = t * r;
			object->matrix.M41 = object->pos.X();
			object->matrix.M42 = object->pos.Y();
			object->matrix.M43 = object->pos.Z();
			object->dirty = false;
		}

		device.transform.model = object->matrix;
		device.transform.Update();
		Render3D::Vertex *mesh = object->mesh;

		for (uint32_t i = 0; i < object->meshNum; i += 3)
		{
			// 切换材质组
			if (object->materialIds == NULL)
			{
				device.material = materials[0];
			}
			else
				device.material = materials[object->materialIds[i / 3]];
			device.ClipPolys(mesh[i], mesh[i + 1], mesh[i + 2], false);
		}
	}
}

int screen_keys[512];	// 当前键盘按下状态
float deltaTime = 0.0f;
Uint32 lastFrame = 0;

int main(int argc, char * argv[])
{
	bool sdl_ret = sdl_init(SCREEN_WIDTH, SCREEN_HEIGHT, "jinqifeng");

	//Start up SDL and create window
	if (sdl_ret == false)
	{
		printf("Failed to initialize!\n");
	}
	else
	{

		//Main loop flag
		bool quit = false;

		Render3D::Raster device;
		device.Init();
		int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
		int indicator = 0;
		int kbhit = 0;

		// 加载初始化纹理和网格和材质数据
		InitTexture();
		Base3D::Material material(NULL, { 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 0.2f, 0.2f, 0.2f },
		{ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, 32.0f, 1.0f, 1.0f, 1, 1, NULL, -1,
		NULL, 2, NULL, -1, NULL, -1, NULL, -1, NULL, -1, NULL, -1);
		materials.push_back(material);

		Render3D::Vertex *mesh_nan;
		unsigned long mesh_num_nan;
		int *material_ids_nan;
		unsigned long material_ids_num_nan;
		Utils3D::MakeMeshAndMaterialByObj(&mesh_nan, mesh_num_nan, &material_ids_nan, material_ids_num_nan, "nanosuit");

		// 缓存
		uint32_t *framebuffer = (uint32_t*)malloc(REAL_WIDTH * REAL_HEIGHT * sizeof(uint32_t));
		float *zbuffer = (float*)malloc(REAL_HEIGHT * REAL_WIDTH * sizeof(float));
		float *shadowbuffer = (float*)malloc(REAL_HEIGHT * REAL_WIDTH * sizeof(float));
		pShadowBuffer = shadowbuffer;

		float c_yaw = 0.0f;
		float c_pitch = 0.0f;
		float c_movementspeed = 2.0f;
		float c_mouse_sensitivity = 0.7f;
		float c_lastX = SCREEN_WIDTH >> 1, c_lastY = SCREEN_HEIGHT >> 1;
		bool firstMouse = true;

		memset(screen_keys, 0, sizeof(int) * 512);

		//dirLight = Base3D::DirLight(Math3D::Vector4(0.0f, 0.0f, 0.0f, 0.0f), Base3D::Color(0.0f, 0.0f, 0.0f, 0.0f), Base3D::Color(0.0f, 0.0f, 0.0f, 0.0f), Base3D::Color(0.0f, 0.0f, 0.0f, 0.0f), false);
		dirLight = Base3D::DirLight(Math3D::Vector4(0.0f, -1.0f, 1.0f, 0.0f), Base3D::Color(0.3f, 0.3f, 0.3f, 1.0f), Base3D::Color(0.8f, 0.8f, 0.8f, 1.0f), Base3D::Color(0.3f, 0.3f, 0.3f, 1.0f), false);
		if (dirLight.shadow == true)
		{
			// 影子摄像机
			Base3D::Camera camera;
			camera.position = Math3D::Vector4(0.0f, 3.0f, -3.0f, 1.0f);
			camera.front = dirLight.direction;
			camera.worldup = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
			camera.fovy = 3.1415926f * 0.5f;
			camera.zNear = 0.1f;
			camera.zFar = 15.0f;
			camera.width = REAL_WIDTH;
			camera.height = REAL_HEIGHT;
			camera.aspect = (float)REAL_WIDTH / (float)REAL_HEIGHT;
			camera.projection = Base3D::orthographic;

			camera.left = -3.0f;
			camera.right = 3.0f;
			camera.bottom = -3.0f;
			camera.top = 3.0f;
			camera.dirty = true;
			camera.InitProjection();
			cameras.push_back(camera);
		}
		/*pointLights.push_back(
			Base3D::PointLight(Math3D::Vector4(0.0f, 6.0f, -1.0f, 1.0f), 1.0f, 0.09f, 0.032f, Base3D::Color(0.6f, 0.6f, 0.6f, 1.0f), Base3D::Color(0.8f, 0.8f, 0.8f, 1.0f), Base3D::Color(0.7f, 0.7f, 0.7f, 1.0f), false)
			);*/
		pointLights.push_back(
			Base3D::PointLight(Math3D::Vector4(0.0f, 6.0f, 2.0f, 1.0f), 1.0f, 0.09f, 0.032f, Base3D::Color(0.6f, 0.6f, 0.6f, 1.0f), Base3D::Color(0.8f, 0.8f, 0.8f, 1.0f), Base3D::Color(0.6f, 0.6f, 0.6f, 1.0f), false)
            );

		// 主摄像机
		Base3D::Camera camera;
		camera.mainCamera = true;
		camera.position = Math3D::Vector4(0.0f, 2.0f, -2.5f, 1.0f);
		camera.front = Math3D::Vector4(0.0f, 0.0f, 1.0f, 0.0f);
		camera.worldup = Math3D::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
		camera.fovy = 3.1415926f * 0.5f;
		camera.zNear = 0.1f;
		camera.zFar = 500.0f;
		camera.width = REAL_WIDTH;
		camera.height = REAL_HEIGHT;
		camera.aspect = (float)REAL_WIDTH / (float)REAL_HEIGHT;
		camera.projection = Base3D::perspective;
		camera.left = -1.0f;
		camera.right = 1.0f;
		camera.bottom = -1.0f;
		camera.top = 1.0f;
		camera.InitProjection();
		camera.dirty = true;
		cameras.push_back(camera);

		device.SetFrameBuffer(framebuffer);
		device.SetZBuffer(zbuffer);
		device.SetShadowBuffer(shadowbuffer);

		device.SetBackground(0xfffffff);

		device.SetCamera(camera);
		device.transform.Update();

		// init object
		// ground
		/*Base3D::Object ground;
		ground.pos = Render3D::Point(0, 0, 0, 1);
		ground.scale = Math3D::Vector4(20, 1, 20, 0);
		ground.axis = Math3D::Vector4(0, 0, 0, 1);
		ground.theta = 0.0f;
		ground.mesh = ground_mesh;
		ground.meshNum = 6;
		ground.materialIds = NULL;
		ground.textureId = 1;
		ground.shadow = false;
		ground.dirty = true;
		objs.push_back(ground);*/

		/*Base3D::Object box;
		box.pos = Render3D::Point(-1, 0, 0, 1);
		box.scale = Math3D::Vector4(0.1f, 0.1f, 0.1f, 0);
		box.axis = Math3D::Vector4(0, 1, 0, 1);
		box.theta = 0.0f;
		box.mesh = mesh_nan;
		box.meshNum = mesh_num_nan;
		box.materialIds = material_ids_nan;
		box.textureId = 1;
		box.shadow = true;
		box.dirty = true;*/
		//objs.push_back(box);
		// box
		

		// box
		Base3D::Object box1;
		box1.pos = Render3D::Point(0, 2, 10, 1);
		box1.scale = Math3D::Vector4(1, 1, 1, 0);
		box1.axis = Math3D::Vector4(1, 0, 1, 1);
		box1.theta = 0.0f;
		box1.mesh = box_mesh;
		box1.meshNum = 36;
		box1.materialIds = NULL;
		box1.textureId = 0;
		box1.shadow = false;
		box1.dirty = true;
		objs.push_back(box1);

		Base3D::Object controlObj = box1;

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit)
		{
			// Set frame time
			Uint32 currentFrame = SDL_GetTicks();
			deltaTime = (currentFrame - lastFrame) * 1.0f / 1000;
			lastFrame = currentFrame;

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//User presses a key
				else if (e.type == SDL_KEYDOWN)
				{
					screen_keys[e.key.keysym.scancode] = 1;
				}
				else if (e.type == SDL_KEYUP)
				{
					screen_keys[e.key.keysym.scancode] = 0;
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					if (firstMouse) {
						c_lastX = e.motion.x;
						c_lastY = e.motion.y;
						firstMouse = false;
					}
					float xoffset = e.motion.x - c_lastX;
					float yoffset = e.motion.y - c_lastY;
					c_lastX = e.motion.x;
					c_lastY = e.motion.y;

					xoffset *= c_mouse_sensitivity;
					yoffset *= c_mouse_sensitivity;

					c_yaw += xoffset;
					c_pitch += yoffset;
					if (c_pitch > 89.0f)
						c_pitch = 89.0f;
					if (c_pitch < -89.0f)
						c_pitch = -89.0f;

					//c_front = (vector_t){e.motion.x*2.0/SCREEN_WIDTH-1, 1-e.motion.y*2.0/SCREEN_HEIGHT, 1, 1};

					camera.dirty = true;
				}
			}

			if (screen_keys[SDL_SCANCODE_W]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp = camera.front;
				temp = temp * velocity;
				camera.position = camera.position + temp;
				camera.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_S])
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp = camera.front;
				temp = temp * velocity;
				camera.position = camera.position - temp;
				camera.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_A]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp;
				temp = Math3D::Cross(camera.front, camera.worldup);
				temp = Math3D::Normalize(temp);
				temp = temp * velocity;
				camera.position = camera.position + temp;
				camera.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_D])
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp;
				temp = Math3D::Cross(camera.front, camera.worldup);
				temp = Normalize(temp);
				temp = temp * velocity;
				camera.position = camera.position - temp;
				camera.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_Q])
			{
				controlObj.theta -= 0.04f;
				controlObj.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_E]) 
			{
				controlObj.theta += 0.04f;
				controlObj.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_UP]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp(0.0f, 1.0f, 0.0f, 0.0f);
				temp = temp  * velocity;
				controlObj.pos = controlObj.pos + temp;
				controlObj.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_LEFT]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp(-1.0f, 0.0f, 0.0f, 0.0f);
				temp = temp * velocity;
				controlObj.pos = controlObj.pos + temp;
				controlObj.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_DOWN]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp(0.0f, -1.0f, 0.0f, 0.0f);
				temp = temp * velocity;
				controlObj.pos = controlObj.pos + temp;
				controlObj.dirty = true;
			}
			if (screen_keys[SDL_SCANCODE_RIGHT]) 
			{
				float velocity = c_movementspeed * deltaTime;
				Math3D::Vector4 temp(1.0f, 0.0f, 0.0f, 0.0f);
				temp = temp * velocity;
				controlObj.pos = controlObj.pos + temp;
				controlObj.dirty = true;
			}

			if (screen_keys[SDL_SCANCODE_SPACE]) 
			{
				if (kbhit == 0) 
				{
					kbhit = 1;
					if (++indicator >= 3) indicator = 0;
					device.renderState = states[indicator];
				}
			}
			else 
			{
				kbhit = 0;
			}

			// box auto rotate
			/*box.theta -= 0.04f;
			box.dirty = true;*/

			// box auto rotate
			box1.theta += 0.04f;
			box1.dirty = true;

			// Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// shadowbuffer在这里设置是为了清空buffer
			device.SetShadowBuffer(shadowbuffer);

			device.Clear();

			if (camera.dirty)
				camera.InitByEuler(c_yaw, c_pitch);

			for (uint32_t i = 0; i < cameras.size(); i++)
			{
				Base3D::Camera camera = cameras[i];
				if (camera.mainCamera == true)
				{
					device.cull = 1;
					device.SetFrameBuffer(framebuffer);
					device.SetZBuffer(zbuffer);
					device.SetShadowBuffer(NULL);
				}
				else 
				{
					device.cull = 2;
					device.SetFrameBuffer(NULL);
					device.SetZBuffer(NULL);
					device.SetShadowBuffer(shadowbuffer);
				}

				if (camera.dirty == true)
				{
					camera.Update();
					camera.dirty = false;
				}
				device.SetCamera(camera);
				device.transform.Update();
				DrawObject(device, objs);
			}

			// 渲染阴影 采用创建mesh方式，只能投影到平面上
			//             draw_shadow(&device, objects, object_count);


			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				for (int x = 0; x < SCREEN_WIDTH; x++)
				{
					uint32_t color = framebuffer[y * REAL_WIDTH + x];
					SDL_SetRenderDrawColor(gRenderer, (0xff << 16 & color) >> 16, (0xff << 8 & color) >> 8, 0xff & color, (0xff << 24 & color) >> 24);
					SDL_RenderDrawPoint(gRenderer, x, y);
				}
			}

			//Update screen
			SDL_RenderPresent(gRenderer);
		}

		free(mesh_nan);
		free(material_ids_nan);
		FreeTextures();
		free(framebuffer);
		free(zbuffer);
		free(shadowbuffer);
	}
	sdl_close();
	return 0;
}