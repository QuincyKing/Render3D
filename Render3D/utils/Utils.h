#pragma once

#include "../render/Primitive.h"
#include "../base/Texture.h"
#include "../math/Vector.h"
#include "../base/Texcoord.h"
#include <cstdint>
#include <cmath>
#include <vector>
#include <cassert>
#include <cstring>
#include "png.h"
#include "tinyobj_loader_c.h"
#include "../base/Material.h"
#include <cstdio>

extern std::vector<Base3D::Texture> textures;
extern std::vector<Base3D::Material> materials;
namespace Utils3D
{
	const char* getFilePath(const char* name, const char* type);
	int GenerateMipmaps(Base3D::Texture &texture, float gamma);
	int MakeMeshAndMaterialByObj(Render3D::Vertex **mesh, unsigned long &mesh_num, int **material_ids, unsigned long &material_ids_num, const char *name);
	int MakeTextureByPng(const char *name, bool mipmap);
}



