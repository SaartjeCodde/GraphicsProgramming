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

#include "HardwareSkinningScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"


HardwareSkinningScene::HardwareSkinningScene(void) :
	GameScene(L"HardwareSkinningScene"),
	m_pModel(nullptr)
{
}


HardwareSkinningScene::~HardwareSkinningScene(void)
{
}

void HardwareSkinningScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);

	m_pModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pModel->SetMaterial(0);
	auto obj = new GameObject();
	obj->AddComponent(m_pModel);
	AddChild(obj);

	obj->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	//Input
	gameContext.pInput->AddInputAction(InputAction(0, Released, 'R'));
	gameContext.pInput->AddInputAction(InputAction(1, Released, 'P'));
	gameContext.pInput->AddInputAction(InputAction(2, Released, 'O'));

	gameContext.pInput->AddInputAction(InputAction(3, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(4, Down, VK_DOWN));
}

void HardwareSkinningScene::Update(const GameContext& gameContext)
{
	auto animator = m_pModel->GetAnimator();

	UNREFERENCED_PARAMETER(gameContext);
	if (gameContext.pInput->IsActionTriggered(0))
	{
		animator->Reset();
	}

	if (gameContext.pInput->IsActionTriggered(1))
	{
		if (animator->IsPlaying())
		{
			animator->Pause();
		}
		else
		{
			animator->Play();
		}
	}

	if (gameContext.pInput->IsActionTriggered(2))
	{
		animator->SetPlayReversed(!animator->IsReversed());
	}

	if (gameContext.pInput->IsActionTriggered(3))
	{
		auto speed = animator->GetAnimationSpeed();
		speed += 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}

	if (gameContext.pInput->IsActionTriggered(4))
	{
		auto speed = animator->GetAnimationSpeed();
		speed -= 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}
}

void HardwareSkinningScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
