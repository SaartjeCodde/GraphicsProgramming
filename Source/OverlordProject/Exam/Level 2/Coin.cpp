#include "Base\stdafx.h"
#include "Coin.h"

#include "Components\Components.h"
#include "Physx/PhysxManager.h"
#include "Content/ContentManager.h"
#include "Components/ModelComponent.h"

Coin::Coin(bool isTrigger):
	m_IsTrigger(isTrigger),
	m_WasTriggered(false),
	m_EndTriggering(false)
{
}

Coin::~Coin(void)
{
}

void Coin::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto defaultMaterial = physX->createMaterial(0, 0, 0);
		
	ModelComponent* model = new ModelComponent(L"./Resources/Meshes/Coin.ovm");
	model->SetMaterial(15);
	
	auto rigidbody = new RigidBodyComponent(true);

	std::shared_ptr<PxGeometry> geom(new PxBoxGeometry(5, 5, 5));
	auto collider = new ColliderComponent(geom,*defaultMaterial);

	if (m_IsTrigger) 
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

	AddComponent(model);
}

void Coin::Update(const GameContext& gameContext)
{	
}

void Coin::Reset()
{
	m_WasTriggered = false;
	m_EndTriggering = true;
}

