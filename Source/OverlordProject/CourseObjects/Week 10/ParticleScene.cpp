//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.121
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"

#include "ParticleScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Graphics\TextureData.h"
#include "Components\TransformComponent.h"
#include "Components\ParticleEmitterComponent.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"
#include "../../Materials/ColorMaterial.h"

ParticleScene::ParticleScene(void) :
	GameScene(L"ParticleScene"),
	m_pRootObject(nullptr)
{
}


ParticleScene::~ParticleScene(void)
{
}

void ParticleScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pRootObject = new GameObject();
	AddChild(m_pRootObject);

	auto particleObj = new GameObject();
	auto particleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 60);
	particleEmitter->SetVelocity(XMFLOAT3(0.0f, 6.0f, 0));
	particleEmitter->SetMinSize(1.0f);
	particleEmitter->SetMaxSize(2.0f);
	particleEmitter->SetMinEnergy(1.0f);
	particleEmitter->SetMaxEnergy(2.0f);
	particleEmitter->SetMinSizeGrow(3.5f);
	particleEmitter->SetMaxSizeGrow(5.5f);
	particleEmitter->SetMinEmitterRange(0.2f);
	particleEmitter->SetMaxEmitterRange(0.5f);
	particleEmitter->SetColor(XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
	particleObj->AddComponent(particleEmitter);	
	m_pRootObject->AddChild(particleObj);

	auto mat = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(mat, 0);

	auto modelObj = new GameObject();
	auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	model->SetMaterial(0);
	modelObj->AddComponent(model);
	m_pRootObject->AddChild(modelObj);
	modelObj->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
}

void ParticleScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto translation = sin(gameContext.pGameTime->GetTotal()) * 5.f;
	m_pRootObject->GetTransform()->Translate(translation, 0.f, 0.f);
}

void ParticleScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
