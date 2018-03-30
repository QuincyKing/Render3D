#pragma once

#include "Color.h"
#include "../math/MathUtil.h"
#include <cstdint>

namespace Base3D
{
	class Texture
	{
	public:
		uint32_t **datas;            // data
		uint32_t datasLen;
		bool useMipmap;            // able mipmap
		uint32_t width;
		uint32_t height;

	public:
		uint32_t TextureValueRead(float u, float v) const
		{
			uint32_t* data = datas[0];
			int _width, _height;
			_width = this->width;
			_height = this->height;
			u = (u - (int)u) * (_width - 1);
			v = (v - (int)v) * (_height - 1);
			int uint = (int)u;
			int vint = (int)v;
			uint32_t res = data[vint*_width + uint];
			return res;
		}

		Base3D::Color TextureRead(float u, float v, float z, float maxz) const
		{
			Base3D::Color color;
			uint32_t* data = datas[0];
			int _width, _height;
			_width = this->width;
			_height = this->height;
			if (useMipmap) 
			{
				int tmiplevels = Math3D::LogBase2OfX(width);
				int miplevel = int(tmiplevels * (z / maxz));
				if (miplevel > tmiplevels) miplevel = tmiplevels;
				data = (uint32_t*)datas[miplevel];
				for (int ts = 0; ts < miplevel; ts++) 
				{
					_width = _width >> 1;
					_height = _height >> 1;
				}
			}
			// wrap ·½Ê½
			u = (u - (int)u) * (_width - 1);
			v = (v - (int)v) * (_height - 1);
			int uint = (int)u;
			int vint = (int)v;
			int uint_pls_1 = uint + 1;
			int vint_pls_1 = vint + 1;
			uint_pls_1 = Clamp(uint_pls_1, 0, _width - 1);
			vint_pls_1 = Clamp(vint_pls_1, 0, _height - 1);

			int textel00, textel10, textel01, textel11;

			textel00 = data[(vint + 0)*_width + (uint + 0)];
			textel10 = data[(vint_pls_1)*_width + (uint + 0)];
			textel01 = data[(vint + 0)*_width + (uint_pls_1)];
			textel11 = data[(vint_pls_1)*_width + (uint_pls_1)];

			int textel00_a = (textel00 >> 24) & 0xff;
			int textel00_r = (textel00 >> 16) & 0xff;
			int textel00_g = (textel00 >> 8) & 0xff;
			int textel00_b = textel00 & 0xff;

			int textel10_a = (textel10 >> 24) & 0xff;
			int textel10_r = (textel10 >> 16) & 0xff;
			int textel10_g = (textel10 >> 8) & 0xff;
			int textel10_b = textel10 & 0xff;

			int textel01_a = (textel01 >> 24) & 0xff;
			int textel01_r = (textel01 >> 16) & 0xff;
			int textel01_g = (textel01 >> 8) & 0xff;
			int textel01_b = textel01 & 0xff;

			int textel11_a = (textel11 >> 24) & 0xff;
			int textel11_r = (textel11 >> 16) & 0xff;
			int textel11_g = (textel11 >> 8) & 0xff;
			int textel11_b = textel11 & 0xff;

			float dtu = u - (float)uint;
			float dtv = v - (float)vint;

			float one_minus_dtu = 1.0f - dtu;
			float one_minus_dtv = 1.0f- dtv;

			float one_minus_dtu_x_one_minus_dtv = (one_minus_dtu)* (one_minus_dtv);
			float dtu_x_one_minus_dtv = (dtu)* (one_minus_dtv);
			float dtu_x_dtv = (dtu)* (dtv);
			float one_minus_dtu_x_dtv = (one_minus_dtu)* (dtv);

			color.a = one_minus_dtu_x_one_minus_dtv * textel00_a +
				dtu_x_one_minus_dtv * textel01_a +
				dtu_x_dtv * textel11_a +
				one_minus_dtu_x_dtv * textel10_a;

			color.r = one_minus_dtu_x_one_minus_dtv * textel00_r +
				dtu_x_one_minus_dtv * textel01_r +
				dtu_x_dtv * textel11_r +
				one_minus_dtu_x_dtv * textel10_r;

			color.g = one_minus_dtu_x_one_minus_dtv * textel00_g +
				dtu_x_one_minus_dtv * textel01_g +
				dtu_x_dtv * textel11_g +
				one_minus_dtu_x_dtv * textel10_g;

			color.b = one_minus_dtu_x_one_minus_dtv * textel00_b +
				dtu_x_one_minus_dtv * textel01_b +
				dtu_x_dtv * textel11_b +
				one_minus_dtu_x_dtv * textel10_b;
			return color * (1.0f / 255);
		}
	};
}