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
#include "TutorialScene.h"

#include "Components\Components.h"
#include "Physx\PhysxManager.h"
#include "Physx\PhysxProxy.h"
#include "Prefabs\Prefabs.h"
#include "Scenegraph\GameObject.h"

TutorialScene::TutorialScene(void):
	GameScene(L"TutorialScene"),
	m_pSphere(nullptr)
{
}


TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	AddChild(ground);

	// ADD SPHERE
	m_pSphere = new SpherePrefab();
	m_pSphere->GetTransform()->Translate(0,5,0);

	// Sphere PhysX
	auto rigidbody = new RigidBodyComponent();
	m_pSphere->AddComponent(rigidbody);

	std::shared_ptr<PxGeometry> spheregeom(new PxSphereGeometry(1));
	m_pSphere->AddComponent(new ColliderComponent(spheregeom,*bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));

	AddChild(m_pSphere);

	auto inputAction = InputAction(0, InputTriggerState::Down, 'M');
	gameContext.pInput->AddInputAction(inputAction);
}

void TutorialScene::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsActionTriggered(0))
	{
		m_pSphere->GetTransform()->Translate(0,10,0);
	}
}

void TutorialScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
