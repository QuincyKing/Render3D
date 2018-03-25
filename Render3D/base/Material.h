#pragma once

#include "Color.h"
#include <cstdlib>

namespace Base3D
{ 
	class Material
	{
	public:
		char *name;

		Color ambient;
		Color diffuse;
		Color specular;
		Color transmittance;
		Color emission;
		float shininess;
		float indexOfRefraction;  //ÕÛÉäÂÊ
		float dissolve;
		int illumination;         //ÕÕÃ÷¶È

		int pad0;

		char *ambientTexname;            /* map_Ka */
		int ambientTexId;
		char *diffuseTexname;            /* map_Kd */
		int diffuseTexId;
		char *specularTexname;           /* map_Ks */
		int specularTexId;
		char *specularHighlightTexname; /* map_Ns */
		int specularHighlightTexId;
		char *bumpTexname;               /* map_bump, bump */
		int bumpTexId;
		char *displacementTexname;       /* disp */
		int displacementTexId;
		char *alphaTexname;              /* map_d */
		int alphaTexId;

		~Material()
		{
			free(this->name);
			free(this->alphaTexname);
			free(this->displacementTexname);
			free(this->bumpTexname);
			free(this->specularHighlightTexname);
			free(this->specularTexname);
			free(this->ambientTexname);
			free(this->diffuseTexname);
		}
	};
}
