//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base/stdafx.h"

#include "PostBlur.h"

#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostBlur::m_pDiffuseSRVvariable = nullptr;

PostBlur::PostBlur(void): PostProcessingMaterial(L"./Resources/Effects/Blur.fx", 0, L"Blur")
{

}

PostBlur::~PostBlur(void)
{

}

void PostBlur::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"PostBlur.cpp::LoadEffectVariables() -> \'gTexture\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}
}

void PostBlur::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(rendertarget->GetShaderResourceView());
	}
}

