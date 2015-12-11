#include "Base/stdafx.h"

#include "PostGrayScale.h"

#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostGrayScale::m_pDiffuseSRVvariable = nullptr;

PostGrayScale::PostGrayScale(void): PostProcessingMaterial(L"./Resources/Effects/Grayscale.fx", 0, L"Grayscale")
{
}


PostGrayScale::~PostGrayScale(void)
{
}

void PostGrayScale::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"PostGrey.cpp::LoadEffectVariables() -> \'gTexture\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}
}

void PostGrayScale::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(rendertarget->GetShaderResourceView());
	}
}
