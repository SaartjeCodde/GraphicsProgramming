//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.82
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"

#include "PointLightMaterial.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"

ID3DX11EffectShaderResourceVariable* PointLightMaterial::m_pDiffuseSRVvariable = nullptr;

PointLightMaterial::PointLightMaterial() : Material(L"./Resources/Effects/PointLight.fx"),
	m_pDiffuseTexture(nullptr)
{
	// Point light--position is changed every frame to animate in UpdateScene function.
	for (int i = 0 ; i < 2 ; i++)
	{
		light[i].pos = XMFLOAT3(10.0f, 10.0f, 10.0f);
		light[i].range = 25.0f;
		light[i].att = XMFLOAT3(10.0f, 10.2f, 10.0f);
		light[i].ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//light[i].diffuse = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		
	}
}

PointLightMaterial::~PointLightMaterial()
{

}

void PointLightMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void PointLightMaterial::SetPointLight(PointLight pointLight)
{
	
}

void PointLightMaterial::SetLightPos(XMFLOAT3 position, int index)
{
	light[index].pos = position;
	// m_PointLightPosition = position;
}
//void DiffuseMaterial::SetLightColor(XMFLOAT4 lightColor)
//{
//	lightColor2 = lightColor;
//}
//void DiffuseMaterial::SetVecLightPos(XMFLOAT3 vecLightPos)
//{
//	vecLightPos2 = vecLightPos;
//}
//void DiffuseMaterial::SetLightRange(XMFLOAT2 lightRange)
//{
//	lightRange2 = lightRange;
//}


void PointLightMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}


	//if (!m_pVecEye)
	//{
	//m_pVecEye = m_pEffect->GetVariableByName("vecEye")->AsVector();
	//	if (!m_pVecEye->IsValid())
	//	{
	//		m_pVecEye = NULL;
	//	}
	//}
	//m_pLightColor = m_pEffect->GetVariableByName("lightColor")->AsVector();
	////
	//m_pVecLightPos = m_pEffect->GetVariableByName("vecLightPos")->AsVector();
	////
	//m_pLightRange = m_pEffect->GetVariableByName("LightRange")->AsVector();

	//// 
	//m_pLightPosition = m_pEffect->GetVariableByName("position")->AsVector();
	////
	//m_pLightRange = m_pEffect->GetVariableByName("range")->AsVector();
	////
	//m_pLightAttenuation = m_pEffect->GetVariableByName("attenuation")->AsVector();
	//m_pLightAmbient = m_pEffect->GetVariableByName("ambient")->AsVector();
	//m_pLightDiffuse = m_pEffect->GetVariableByName("diffuse")->AsVector();
	//
		
	//mfxPointLight = m_pEffect->GetVariableByName("light");

	// light.position = m_pEffect->GetVariableByName("position");


	mfxPointLight = m_pEffect->GetVariableByName("light");
	//mfxPointLight2 = m_pEffect->GetVariableByName("light");

	//mfxPointLight2 = m_pEffect->GetVariableByName("light");

}

void PointLightMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
	
	/*light.pos = XMFLOAT3(10.0f, 10.0f, 10.0f);
	light.range = 100.0f;
	light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);
	light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/

	//light.pos = XMFLOAT3( m_PointLightPosition.x,  m_PointLightPosition.y,  m_PointLightPosition.z);

	mfxPointLight->SetRawValue(&light[0], 0, sizeof(light[0]));
// 	mfxPointLight2->SetRawValue(&light[1], 0, sizeof(light[1]));
	//	mfxPointLight2->SetRawValue(&light[1], 0, sizeof(light[1]));

	/*m_pLightPosition = XMVECTOR(0.0f, 0.0f, 0.0f); 
	m_pLightRange = 	
	m_pLightAttenuation = 
	m_pLightAmbient = 
	m_pLightDiffuse = 


	light.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	light.range = 100.0f;
	light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);
	light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/


}
