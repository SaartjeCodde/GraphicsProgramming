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

#include "Wall.h"
#include "Components\Components.h"
#include "Physx/PhysxManager.h"

Wall::Wall(float width, float height, float depth, bool isTrigger):
	m_Dimensions(width,height,depth),
	m_IsTrigger(isTrigger),
	m_WasTriggered(false)
{
}


Wall::~Wall(void)
{
}

void Wall::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto defaultMaterial = physX->createMaterial(0, 0, 1);


	auto rigidbody = new RigidBodyComponent(true);

	std::shared_ptr<PxGeometry> geom(new PxBoxGeometry(m_Dimensions.x/2, m_Dimensions.y/2, m_Dimensions.z/2));
	auto collider = new ColliderComponent(geom,*defaultMaterial);

	if(m_IsTrigger) 
	{
		this->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, TriggerAction action)
											{
												UNREFERENCED_PARAMETER(receiver);
												UNREFERENCED_PARAMETER(trigger);
												if(action == TriggerAction::ENTER)this->m_WasTriggered = true;
											});
		collider->EnableTrigger(true);
	}
	AddComponent(rigidbody);

	AddComponent(collider);
}
