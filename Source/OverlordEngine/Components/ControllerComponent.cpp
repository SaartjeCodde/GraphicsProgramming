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
#include "stdafx.h"

#include "ControllerComponent.h"
#include "TransformComponent.h"
#include "../Physx/PhysxProxy.h"
#include "../Physx/PhysxManager.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Helpers/PhysxHelper.h"
#include "../Diagnostics/Logger.h"

ControllerComponent::ControllerComponent(PxMaterial* material, float radius, float height, wstring name, PxCapsuleClimbingMode::Enum climbingMode) : 
	m_pMaterial(material),
	m_Radius(radius),
	m_Height(height),
	m_Name(name),
	m_ClimbingMode(climbingMode),
	m_Controller(nullptr),
	m_CollisionFlag(PxControllerCollisionFlags()),
	m_CollisionGroups(PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0))
{
}

ControllerComponent::~ControllerComponent()
{
}

void ControllerComponent::Initialize(const GameContext& gameContext)
{
		UNREFERENCED_PARAMETER(gameContext);
	if(m_Controller != nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot initialize a controller twice");

	// 1. Create a PxCapsuleControllerDesc 
	PxCapsuleControllerDesc desc;
	desc.setToDefault();
	desc.radius = m_Radius;
	desc.height = m_Height;
	desc.material = m_pMaterial;
	desc.climbingMode = m_ClimbingMode;
	desc.upDirection = PxVec3(0,1,0);
	desc.contactOffset = 0.1f;
	desc.userData = this;
	auto position = GetGameObject()->GetTransform()->GetPosition();
	desc.position = ToPxExtendedVec3(position);
	
	// 2. Retrieve the controller manager from the PhysXProxy
	auto manager = m_pGameObject->GetScene()->GetPhysxProxy()->GetControllerManager();
	
	// Use the manager to create a PxController
	m_Controller = manager->createController(desc);

	if(m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Failed to create controller");

	// 3. Retrieve the controller actor and give it a name 
	string name(m_Name.begin(), m_Name.end());
	m_Controller->getActor()->setName(name.c_str());

	// +++ Controller bugfix updates +++
	m_Controller->getActor()->userData = this;
	SetCollisionGroup((CollisionGroupFlag) m_CollisionGroups.word0);
	SetCollisionIgnoreGroups((CollisionGroupFlag) m_CollisionGroups.word1);
}

void ControllerComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ControllerComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void ControllerComponent::Translate(XMFLOAT3 position)
{
	if(m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Translate. No controller present");
	else
		m_Controller->setPosition(ToPxExtendedVec3(position));
}

void ControllerComponent::Translate(float x, float y, float z)
{
	Translate(XMFLOAT3(x,y,z));
}

void ControllerComponent::Move(XMFLOAT3 displacement, float minDist)
{
	if(m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Move. No controller present");
	else
		m_CollisionFlag = m_Controller->move(ToPxVec3(displacement), minDist, 0, 0, 0);
}

XMFLOAT3 ControllerComponent::GetPosition()
{
	if(m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get position. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getPosition());

	return XMFLOAT3();
}

XMFLOAT3 ControllerComponent::GetFootPosition()
{
	if(m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get footposition. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getFootPosition());

	return XMFLOAT3();
}

// +++ Controller bugfix updates +++
void ControllerComponent::SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups)//EDIT (Controller-Trigger-Bug)
{
	m_CollisionGroups.word1 = ignoreGroups;

	if (!m_Controller)
		return;

	auto nbShapes = m_Controller->getActor()->getNbShapes();

	PxShape* buffer;
	m_Controller->getActor()->getShapes(&buffer, nbShapes * sizeof(PxShape));

	for (int i = 0; i < nbShapes; ++i)
	{
		buffer[i].setSimulationFilterData(m_CollisionGroups);
	}
}

void ControllerComponent::SetCollisionGroup(CollisionGroupFlag group)//EDIT (Controller-Trigger-Bug)
{
	m_CollisionGroups.word0 = group;

	if (!m_Controller)
		return;

	auto nbShapes = m_Controller->getActor()->getNbShapes();

	PxShape* buffer;
	m_Controller->getActor()->getShapes(&buffer, nbShapes * sizeof(PxShape));

	for (int i = 0; i < nbShapes; ++i)
	{
		buffer[i].setSimulationFilterData(m_CollisionGroups);
	}
}
