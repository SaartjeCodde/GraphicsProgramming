#include "stdafx.h"
#include "SkyBoxPrefab.h"

SkyBoxPrefab::SkyBoxPrefab(wstring assetFile):
	m_AssetFile(assetFile)
{
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto skyBoxModel = new ModelComponent(L"./Resources/Meshes/Box.ovm");
	
	auto skyBoxMaterial = new SkyBoxMaterial();
	gameContext.pMaterialManager->AddMaterial(skyBoxMaterial, 60);

	skyBoxMaterial->SetTexture(m_AssetFile);

	skyBoxModel->SetMaterial(60);

	auto skyBox = new GameObject();
	skyBox->AddComponent(skyBoxModel);

	AddChild(skyBox);
}
