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
#include "ModelTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/ModelComponent.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Components/TransformComponent.h"

ModelTestScene::ModelTestScene(void) :
	GameScene(L"ModelTestScene"),
	m_pChair(nullptr)
{
}

ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//Chair Material
	auto chairMat = new DiffuseMaterial();
	chairMat->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(chairMat, 0);

	//Chair Model Component
	auto chairModel = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	chairModel->SetMaterial(0);
	m_pChair->AddComponent(chairModel);

	//Chair PhysX
	auto chairBody = new RigidBodyComponent();
	m_pChair->AddComponent(chairBody);

	auto chairPxMaterial = physX->createMaterial(0.5f, 0.5f, 0.1f);
	auto chairPxMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");		
	shared_ptr<PxGeometry> chairGeom(new PxConvexMeshGeometry(chairPxMesh));
	auto chairCollider = new ColliderComponent(chairGeom, *chairPxMaterial);
	m_pChair->AddComponent(chairCollider);

	m_pChair->GetTransform()->Translate(0, 10, 0);

	AddChild(m_pChair);
}

void ModelTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
