//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.113
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"

#include "PostProcessingScene.h"

#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"

#include "Components/ModelComponent.h"
#include "../../Materials/DiffuseMaterial.h"

#include "../../Materials/PostgrayScale.h"
#include "../../Materials/PostBlur.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
PostProcessingScene::PostProcessingScene(void):
	GameScene(L"PostProcessScene")
	, m_Blur(nullptr)
	, m_Grey(nullptr)
{
}


PostProcessingScene::~PostProcessingScene(void)
{
}

void PostProcessingScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	DiffuseMaterial* diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 0);

	ModelComponent* model = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	model->SetMaterial(0);

	GameObject* knight = new GameObject();
	knight->AddComponent(model);

	AddChild(knight);

	m_Blur = new PostBlur();
	m_Blur->Initialize(gameContext);
	AddPostProcessingEffect(m_Blur);

	m_Grey = new PostGrayScale();
	m_Grey->Initialize(gameContext);
	AddPostProcessingEffect(m_Grey);	
}

void PostProcessingScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//m_Blur->UpdateEffectVariables();
	
}

void PostProcessingScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

