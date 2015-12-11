#pragma once
#include "Base\stdafx.h"
#include "Components\Components.h"

struct SkinnedVertex
{
	SkinnedVertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float blendWeight1, float blendWeight2) : 
		TransformedVertex(position, normal, color), 
		OriginalVertex(position, normal, color),
		m_BlendWeight1(blendWeight1),
		m_BlendWeight2(blendWeight2)
	{}

	VertexPosNormCol TransformedVertex;
	VertexPosNormCol OriginalVertex;
	float m_BlendWeight1;
	float m_BlendWeight2;
};