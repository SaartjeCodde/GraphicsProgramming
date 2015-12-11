#include "Base/stdafx.h"
#include "CharacterTest.h"
#include "Character.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Physx/PhysxProxy.h"
#include "Physx/PhysxManager.h"

CharacterTest::CharacterTest(void):
GameScene(L"CharacterTest")
{
}

CharacterTest::~CharacterTest(void)
{
}

void CharacterTest::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Ground
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto basicMaterial = physX->createMaterial(0,0,0);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *basicMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0,0,1)))));
	AddChild(ground);

	// Character
	auto characterController = new Character();	
	AddChild(characterController);
}

void CharacterTest::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
