#include "Base\stdafx.h"
#include "Key.h"

#include "Components/Components.h"
#include "Physx/PhysxManager.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Components/ModelComponent.h"

Key::Key(bool isTrigger):
	m_IsTrigger(isTrigger),
	m_WasTriggered(false),
	m_EndTriggering(false)
{
}

Key::~Key(void)
{
}

void Key::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto defaultMaterial = physX->createMaterial(0, 0, 0);
	
	auto material = new DiffuseMaterial();
	material->SetDiffuseTexture(L"./Resources/Textures/Coin.jpg");
	gameContext.pMaterialManager->AddMaterial(material, 99);

	ModelComponent* model = new ModelComponent(L"./Resources/Meshes/Key.ovm");
	model->SetMaterial(99);
	
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

void Key::Update(const GameContext& gameContext)
{	
}

void Key::Reset()
{		
	m_WasTriggered = false;
	m_EndTriggering = true;
}

