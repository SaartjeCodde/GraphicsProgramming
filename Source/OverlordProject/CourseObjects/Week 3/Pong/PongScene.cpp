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

#include "PongScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Peddle.h"
#include "Wall.h"
#include "Ball.h"
#include "Scenegraph\SceneManager.h"
#include <memory>

enum PongInput
{
	RightPeddle_UP = 0,
	RightPeddle_DOWN = 1,
	LeftPeddle_UP = 2,
	LeftPeddle_DOWN = 3,
	LaunchBall = 4
};

PongScene::PongScene(void):
	GameScene(L"PongScene")
{
}


PongScene::~PongScene(void)
{
}

void PongScene::Initialize(const GameContext& gameContext)
{
	// ROTATE DOES NOT SEEM TO WORK with FreeCamera
	//gameContext.pCamera->GetTransform()->Translate(0,1000,0);
	//gameContext.pCamera->GetTransform()->Rotate(90,0,0);
	// FIXME: Fixed Camera

	auto camEmpty = new GameObject();
	auto cam = new CameraComponent();
	camEmpty->GetTransform()->Translate(0,1000,0);
	camEmpty->GetTransform()->Rotate(90,0,0);
	camEmpty->AddComponent(cam);
	AddChild(camEmpty);
	SetActiveCamera(cam);

	//Input Actions
	auto inputAction = InputAction(PongInput::RightPeddle_UP, InputTriggerState::Down, VK_UP);
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(PongInput::RightPeddle_DOWN, InputTriggerState::Down, VK_DOWN);
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(PongInput::LeftPeddle_UP, InputTriggerState::Down, 'Y');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(PongInput::LeftPeddle_DOWN, InputTriggerState::Down, 'H');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(PongInput::LaunchBall, InputTriggerState::Released, VK_SPACE);
	gameContext.pInput->AddInputAction(inputAction);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	
	auto defaultMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*defaultMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	AddChild(ground);
	
	//Right Peddle
	m_pRightPeddle = new Peddle(40,40,160);
	m_pRightPeddle->GetTransform()->Translate(600,0,0);
	AddChild(m_pRightPeddle);

	//Left Peddle
	m_pLeftPeddle = new Peddle(40,40,160);
	m_pLeftPeddle->GetTransform()->Translate(-600,0,0);
	AddChild(m_pLeftPeddle);

	//Top Wall
	m_pTopWall = new Wall(1280, 40, 20);
	m_pTopWall->GetTransform()->Translate(0,0,360);
	AddChild(m_pTopWall);

	//Bottom Wall
	m_pBottomWall = new Wall(1280, 40, 20);
	m_pBottomWall->GetTransform()->Translate(0,0,-360);
	AddChild(m_pBottomWall);

	//Right Wall
	m_pRightWall = new Wall(20,40,690, true);
	m_pRightWall->GetTransform()->Translate(640,0,0);
	AddChild(m_pRightWall);

	//Left Wall
	m_pLeftWall = new Wall(20,40,690, true);
	m_pLeftWall->GetTransform()->Translate(-640,0,0);
	AddChild(m_pLeftWall);

	//Ball
	m_pBall = new Ball(10);
	m_pBall->GetTransform()->Translate(0,5,0);
	AddChild(m_pBall);

}

void PongScene::Update(const GameContext& gameContext)
{
	//RIGHT PEDDLE
	auto rightPeddlePos = m_pRightPeddle->GetTransform()->GetWorldPosition();

	if(gameContext.pInput->IsActionTriggered(PongInput::RightPeddle_UP))
	{
		rightPeddlePos.z += PEDDLE_SPEED*gameContext.pGameTime->GetElapsed();
	}
	else if(gameContext.pInput->IsActionTriggered(PongInput::RightPeddle_DOWN))
	{
		rightPeddlePos.z -= PEDDLE_SPEED*gameContext.pGameTime->GetElapsed();
	}



	Clamp<float>(rightPeddlePos.z, 270.0f, -270.0f);
	m_pRightPeddle->GetTransform()->Translate(rightPeddlePos);

	//LEFT PEDDLE
	auto leftPeddlePos = m_pLeftPeddle->GetComponent<TransformComponent>()->GetWorldPosition();

	if(gameContext.pInput->IsActionTriggered(PongInput::LeftPeddle_UP))
	{
		leftPeddlePos.z += PEDDLE_SPEED*gameContext.pGameTime->GetElapsed();
	}
	else if(gameContext.pInput->IsActionTriggered(PongInput::LeftPeddle_DOWN))
	{
		leftPeddlePos.z -= PEDDLE_SPEED*gameContext.pGameTime->GetElapsed();
	}

	Clamp<float>(leftPeddlePos.z, 270.0f, -270.0f);
	m_pLeftPeddle->GetTransform()->Translate(leftPeddlePos);


	if(m_CanFire && gameContext.pInput->IsActionTriggered(PongInput::LaunchBall))
	{
		m_CanFire = false;
		m_pBall->GetComponent<RigidBodyComponent>()->AddForce(PxVec3(0.7f,0.0f,0.3f)*1000,PxForceMode::eIMPULSE);
	}

	if(m_pLeftWall->IsTriggered() || m_pRightWall->IsTriggered())
	{
		m_pRightPeddle->GetTransform()->Translate(600,0,0);
		m_pLeftPeddle->GetTransform()->Translate(-600,0,0);

		m_pLeftWall->Reset();
		m_pRightWall->Reset();
	
		m_pBall->GetTransform()->Translate(0,5,0);
		m_pBall->GetComponent<RigidBodyComponent>()->PutToSleep();

		m_CanFire = true;
	}
}

void PongScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
