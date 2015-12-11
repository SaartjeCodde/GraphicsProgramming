#pragma once
#include "Graphics/PostProcessingMaterial.h"

class PostGrayScale: public PostProcessingMaterial
{
public:
	PostGrayScale(void);
	~PostGrayScale(void);

	void LoadEffectVariables();
	void UpdateEffectVariables(RenderTarget* rendertarget);

private:
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostGrayScale(const PostGrayScale &obj);
	PostGrayScale& operator=(const PostGrayScale& obj);

};

