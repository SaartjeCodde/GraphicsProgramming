#include "Base\stdafx.h"
#include "SkyBoxMaterial.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pSkyBoxSRVvariable = nullptr;

SkyBoxMaterial::SkyBoxMaterial(void): Material(L"./Resources/Effects/SkyBox.fx"),
	m_pSkyBoxTexture(nullptr)
{
}

SkyBoxMaterial::~SkyBoxMaterial(void)
{
}

void SkyBoxMaterial::SetTexture(const wstring& assetFile)
{
	m_pSkyBoxTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkyBoxMaterial::LoadEffectVariables()
{
	if (!m_pSkyBoxSRVvariable)
	{
		m_pSkyBoxSRVvariable = m_pEffect->GetVariableByName("m_CubeMap")->AsShaderResource();
		if (!m_pSkyBoxSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkyBoxMaterial::LoadEffectVariables() > \'m_CubeMap\' variable not found!");
			m_pSkyBoxSRVvariable = nullptr;
		}
	}
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pSkyBoxTexture && m_pSkyBoxSRVvariable)
	{
		m_pSkyBoxSRVvariable->SetResource(m_pSkyBoxTexture->GetShaderResourceView());
	}
}

