#include "Base\stdafx.h"
#include "SkyBoxTestScene.h"

#include "../../Materials/SkyBoxMaterial.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/ModelComponent.h"
#include "Scenegraph\GameObject.h"
#include "Prefabs\Prefabs.h"

SkyBoxTestScene::SkyBoxTestScene(void):
	GameScene(L"SkyBoxTestScene")	
{	
}


SkyBoxTestScene::~SkyBoxTestScene(void)
{
}

void SkyBoxTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// auto skyBox = new SkyBoxPrefab();
}

void SkyBoxTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyBoxTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
}


