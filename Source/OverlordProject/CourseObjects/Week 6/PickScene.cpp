//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"
#include "PickScene.h"

#include "Components\Components.h"
#include "Physx\PhysxManager.h"
#include "Physx\PhysxProxy.h"
#include "Prefabs\Prefabs.h"
#include "Scenegraph\GameObject.h"

PickScene::PickScene(void) :
GameScene(L"PickScene") {
}


PickScene::~PickScene(void) {
}

void PickScene::Initialize(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);


	// ADD SPHERE
	auto sphere1 = new SpherePrefab();
	sphere1->GetTransform()->Translate(0, 10, 0);

	// Sphere PhysX
	auto rigidbody = new RigidBodyComponent(true);
	sphere1->AddComponent(rigidbody);

	std::shared_ptr<PxGeometry> spheregeom(new PxSphereGeometry(1));
	sphere1->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));

	AddChild(sphere1);

}

void PickScene::Update(const GameContext& gameContext) {
	if (gameContext.pInput->IsMouseButtonDown(VK_RBUTTON) && !gameContext.pInput->IsMouseButtonDown(VK_RBUTTON, true)) {
		GameObject* pickedObject = gameContext.pCamera->Pick(gameContext);
		if (pickedObject != nullptr) {
			RemoveChild(pickedObject);
			pickedObject = nullptr;
		}
	}
}

void PickScene::Draw(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);
}
