#pragma once

#include "Color.h"
#include <cstdlib>

namespace Base3D
{ 
	class Material
	{
	public:
		Material() = default;
		Material(char *name, Color amb, Color diff, Color spe, Color trans, Color emi, float shi, float iof, float diss, int ill, int pad,
			char *ambientTexname, int ambientTexId, char *diffuseTexname, int diffuseTexId, char *specularTexname, int specularTexId,
			char *specularHighlightTexname, int specularHighlightTexId, char *bumpTexname, int bumpTexId,
			char *displacementTexname, int displacementTexId, char *alphaTexname, int alphaTexId)
			:name(name), ambient(amb), diffuse(diff), specular(spe), transmittance(trans), emission(emi),
			shininess(shi), indexOfRefraction(iof), dissolve(diss), illumination(ill), pad0(pad),
			ambientTexname(ambientTexname), ambientTexId(ambientTexId), diffuseTexname(diffuseTexname),
			diffuseTexId(diffuseTexId), specularHighlightTexname(specularHighlightTexname), specularHighlightTexId(specularHighlightTexId),
			bumpTexname(bumpTexname), bumpTexId(bumpTexId), displacementTexname(displacementTexname), displacementTexId(displacementTexId),
			alphaTexname(alphaTexname), alphaTexId(alphaTexId)
		{

		}

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
