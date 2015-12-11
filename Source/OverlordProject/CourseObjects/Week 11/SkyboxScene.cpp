#include "Base\stdafx.h"
#include "SkyboxScene.h"
#include "../../../OverlordEngine/Prefabs/SkyBoxPrefab.h"

SkyboxScene::SkyboxScene(void):
	GameScene(L"SkyboxScene")	
{	
}

SkyboxScene::~SkyboxScene(void)
{
}

void SkyboxScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
		
	auto skyBox = new SkyBoxPrefab(L"Resources/Textures/SkyBox.dds");
	AddChild(skyBox);
}

void SkyboxScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyboxScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
}


