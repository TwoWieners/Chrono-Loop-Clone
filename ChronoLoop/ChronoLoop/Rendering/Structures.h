#pragma once
//#include "stdafx.h"
#include "../Common/Math.h"

namespace Epoch
{

	struct VertexPos
	{
		vec4f	Position;
	};
	struct VertexPosColor
	{
		vec4f	Position;
		vec4f	Color;
	};
	struct VertexPosTex
	{
		vec4f	Position;
		vec4f	UV;
	};
	struct VertexPosNormTex
	{
		vec4f	Position;
		vec4f	Normal;
		vec4f	UV;
	};
	struct VertexPosNormTanTex
	{
		vec4f Position;
		vec4f Normal;
		vec4f UV;
		vec4f Tangent;
	};
	struct VertexPosBoneWeight
	{
		vec4f	Position;
		vec4i	BoneIndices;
		vec4f	Weights;
	};
	struct VertexPosBoneWeightNormTex
	{
		vec4f	Position;
		vec4i	BoneIndices;
		vec4f	Weights;
		vec4f	Normal;
		vec4f	UV;
		VertexPosBoneWeightNormTex() {};
	};
	struct VertexPosBoneWeightNormTanTex
	{
		vec4f	Position;
		vec4f	Normal;
		vec4f	UV;
		vec4i	BoneIndices;
		vec4f	Weights;
		vec4f	Tangent;
		float	Determinant;
		VertexPosBoneWeightNormTanTex() {};
	};

	struct Light
	{
	public:
		int Type;
		float pad[3];

		vec3f Position;

		vec3f Direction;

		vec3f ConeDirection;

		vec3f Color;

		float ConeRatio;
		float Range;
		float pad1[2];

		Light()
		{
			//TODO: idk
		}
	};

}