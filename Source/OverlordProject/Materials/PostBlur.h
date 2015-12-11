#pragma once
#include "Graphics/PostProcessingMaterial.h"

class PostBlur: public PostProcessingMaterial
{
public:
	PostBlur(void);
	~PostBlur(void);

	void LoadEffectVariables();
	void UpdateEffectVariables(RenderTarget* rendertarget);

private:
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostBlur(const PostBlur &obj);
	PostBlur& operator=(const PostBlur& obj);
};

