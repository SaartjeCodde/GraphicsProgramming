#include "Base/stdafx.h"
#include "Environment.h"

#include "../../../OverlordEngine/Physx/PhysxManager.h"
#include "../../Materials/DiffuseMaterial.h"
#include "../../../OverlordEngine/Components/ModelComponent.h"
#include "../../../OverlordEngine/Components/RigidBodyComponent.h"
#include "../../../OverlordEngine/Components/ColliderComponent.h"
#include "../../../OverlordEngine/Components/ParticleEmitterComponent.h"
#include "../../../OverlordEngine/Components/TransformComponent.h"
#include "../../../OverlordEngine/Content/ContentManager.h"

Environment::Environment():
	m_pMillPropellersModel(nullptr),
	m_Counter(0.0)
{
}

Environment::~Environment(void)
{
}

void Environment::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto standardMaterial = physX->createMaterial(0, 0, 0);

	// *********************
	// * Environment setup *
	// *********************
	
	// 1. Terrain
	auto terrainMaterial = new DiffuseMaterial();
	terrainMaterial->SetDiffuseTexture(L"./Resources/Textures/Terrain.jpg");
	gameContext.pMaterialManager->AddMaterial(terrainMaterial, 1);

	ModelComponent* terrainModel = new ModelComponent(L"./Resources/Meshes/Terrain.ovm");
	terrainModel->SetMaterial(1);

	GameObject* terrain = new GameObject();
	terrain->AddComponent(terrainModel);

	auto terrainRigidBody = new RigidBodyComponent(true);
	terrain->AddComponent(terrainRigidBody);
	
	auto terrainMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Terrain.ovpt");		

	shared_ptr<PxGeometry> terrainGeometry(new PxTriangleMeshGeometry(terrainMesh));
	auto terrainCollider = new ColliderComponent(terrainGeometry,*standardMaterial);
	terrain->AddComponent(terrainCollider);

	AddChild(terrain);

	// 2. Cave stones
	auto caveStonesMaterial = new DiffuseMaterial();
	caveStonesMaterial->SetDiffuseTexture(L"./Resources/Textures/CaveStones.jpg");
	gameContext.pMaterialManager->AddMaterial(caveStonesMaterial, 2);

	ModelComponent* caveStonesModel = new ModelComponent(L"./Resources/Meshes/CaveStones.ovm");
	caveStonesModel->SetMaterial(2);

	GameObject* caveStones = new GameObject();
	caveStones->AddComponent(caveStonesModel);

	auto caveStonesRigidBody = new RigidBodyComponent(true);
	caveStones->AddComponent(caveStonesRigidBody);
	
	auto caveStonesMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/CaveStones.ovpt");		

	shared_ptr<PxGeometry> caveStonesGeometry(new PxTriangleMeshGeometry(caveStonesMesh));
	auto caveStonesCollider = new ColliderComponent(caveStonesGeometry,*standardMaterial);
	caveStones->AddComponent(caveStonesCollider);

	AddChild(caveStones);

	// 3. Cave entrance
	auto caveEntranceMaterial = new DiffuseMaterial();
	caveEntranceMaterial->SetDiffuseTexture(L"./Resources/Textures/CaveEntrance.jpg");
	gameContext.pMaterialManager->AddMaterial(caveEntranceMaterial, 3);

	ModelComponent* caveEntranceModel = new ModelComponent(L"./Resources/Meshes/CaveEntrance.ovm");
	caveEntranceModel->SetMaterial(3);

	GameObject* caveEntrance = new GameObject();
	caveEntrance->AddComponent(caveEntranceModel);

	auto caveEntranceRigidBody = new RigidBodyComponent(true);
	caveEntrance->AddComponent(caveEntranceRigidBody);
	
	auto caveEntranceMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/CaveEntrance.ovpc");		

	shared_ptr<PxGeometry> caveEntranceGeometry(new PxConvexMeshGeometry(caveEntranceMesh));
	auto caveEntranceCollider = new ColliderComponent(caveEntranceGeometry,*standardMaterial);
	caveEntrance->AddComponent(caveEntranceCollider);
		
	AddChild(caveEntrance);

	// 4. Cave statues
	auto caveStatuesMaterial = new DiffuseMaterial();
	caveStatuesMaterial->SetDiffuseTexture(L"./Resources/Textures/CaveStatues.jpg");
	gameContext.pMaterialManager->AddMaterial(caveStatuesMaterial, 4);

	ModelComponent* caveStatuesModel = new ModelComponent(L"./Resources/Meshes/CaveStatues.ovm");
	caveStatuesModel->SetMaterial(4);

	GameObject* caveStatues = new GameObject();
	caveStatues->AddComponent(caveStatuesModel);

	auto caveStatuesRigidBody = new RigidBodyComponent(true);
	caveStatues->AddComponent(caveStatuesRigidBody);
	
	auto caveStatuesMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/CaveStatues.ovpt");		

	shared_ptr<PxGeometry> caveStatuesGeometry(new PxTriangleMeshGeometry(caveStatuesMesh));
	auto caveStatuesCollider = new ColliderComponent(caveStatuesGeometry,*standardMaterial);
	caveStatues->AddComponent(caveStatuesCollider);

	AddChild(caveStatues);
	
	// 5. Wood sign
	auto woodSignMaterial = new DiffuseMaterial();
	woodSignMaterial->SetDiffuseTexture(L"./Resources/Textures/WoodSign.jpg");
	gameContext.pMaterialManager->AddMaterial(woodSignMaterial, 5);

	ModelComponent* woodSignModel = new ModelComponent(L"./Resources/Meshes/WoodSign.ovm");
	woodSignModel->SetMaterial(5);

	GameObject* woodSign = new GameObject();
	woodSign->AddComponent(woodSignModel);

	auto woodSignRigidBody = new RigidBodyComponent(true);
	woodSign->AddComponent(woodSignRigidBody);
	
	auto woodSignMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/WoodSign.ovpc");		

	shared_ptr<PxGeometry> woodSignGeometry(new PxConvexMeshGeometry(woodSignMesh));
	auto woodSignCollider = new ColliderComponent(woodSignGeometry,*standardMaterial);
	woodSign->AddComponent(woodSignCollider);

	AddChild(woodSign);

	// 6. Trees
	auto treeBarkMaterial = new DiffuseMaterial();
	treeBarkMaterial->SetDiffuseTexture(L"./Resources/Textures/TreeBark.jpg");
	gameContext.pMaterialManager->AddMaterial(treeBarkMaterial, 6);

	ModelComponent* treesModel = new ModelComponent(L"./Resources/Meshes/Trees.ovm");
	treesModel->SetMaterial(6);

	GameObject* trees = new GameObject();
	trees->AddComponent(treesModel);

	auto treesRigidBody = new RigidBodyComponent(true);
	trees->AddComponent(treesRigidBody);
	
	auto treesMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Trees.ovpt");		

	shared_ptr<PxGeometry> treesGeometry(new PxTriangleMeshGeometry(treesMesh));
	auto treesCollider = new ColliderComponent(treesGeometry,*standardMaterial);
	trees->AddComponent(treesCollider);
	
	AddChild(trees);
	
	// 7. Log
	ModelComponent* logModel = new ModelComponent(L"./Resources/Meshes/Log.ovm");
	logModel->SetMaterial(6);

	GameObject* log = new GameObject();
	log->AddComponent(logModel);

	auto logRigidBody = new RigidBodyComponent(true);
	log->AddComponent(logRigidBody);
	
	auto logMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Log.ovpc");		

	shared_ptr<PxGeometry> logGeometry(new PxConvexMeshGeometry(logMesh));
	auto logCollider = new ColliderComponent(logGeometry,*standardMaterial);
	log->AddComponent(logCollider);
	
	AddChild(log);

	// 8. Campfires
	auto campfiresMaterial = new DiffuseMaterial();
	campfiresMaterial->SetDiffuseTexture(L"./Resources/Textures/Campfires.jpg");
	gameContext.pMaterialManager->AddMaterial(campfiresMaterial, 7);

	ModelComponent* campfiresModel = new ModelComponent(L"./Resources/Meshes/Campfires.ovm");
	campfiresModel->SetMaterial(7);

	GameObject* campfires = new GameObject();
	campfires->AddComponent(campfiresModel);
	
	AddChild(campfires);

		// *** Particles ***
		auto particleObj1 = new GameObject();
		auto particleEmitter1 = new ParticleEmitterComponent(L"./Resources/Textures/Fire.png", 60);
		particleEmitter1->SetVelocity(XMFLOAT3(0.0f, 6.0f, 0));
		particleEmitter1->SetMinSize(1.0f);
		particleEmitter1->SetMaxSize(2.0f);
		particleEmitter1->SetMinEnergy(1.0f);
		particleEmitter1->SetMaxEnergy(2.0f);
		particleEmitter1->SetMinSizeGrow(3.5f);
		particleEmitter1->SetMaxSizeGrow(5.5f);
		particleEmitter1->SetMinEmitterRange(0.2f);
		particleEmitter1->SetMaxEmitterRange(0.5f);
		particleObj1->AddComponent(particleEmitter1);	
		particleObj1->GetTransform()->Translate(-72,-6,10);
		campfires->AddChild(particleObj1);

		auto particleObj2 = new GameObject();
		auto particleEmitter2 = new ParticleEmitterComponent(L"./Resources/Textures/Fire.png", 60);
		particleEmitter2->SetVelocity(XMFLOAT3(0.0f, 6.0f, 0));
		particleEmitter2->SetMinSize(1.0f);
		particleEmitter2->SetMaxSize(2.0f);
		particleEmitter2->SetMinEnergy(1.0f);
		particleEmitter2->SetMaxEnergy(2.0f);
		particleEmitter2->SetMinSizeGrow(3.5f);
		particleEmitter2->SetMaxSizeGrow(5.5f);
		particleEmitter2->SetMinEmitterRange(0.2f);
		particleEmitter2->SetMaxEmitterRange(0.5f);
		particleObj2->AddComponent(particleEmitter2);	
		particleObj2->GetTransform()->Translate(86,-7,-103);
		campfires->AddChild(particleObj2);

		auto particleObj3 = new GameObject();
		auto particleEmitter3 = new ParticleEmitterComponent(L"./Resources/Textures/Fire.png", 60);
		particleEmitter3->SetVelocity(XMFLOAT3(0.0f, 6.0f, 0));
		particleEmitter3->SetMinSize(1.0f);
		particleEmitter3->SetMaxSize(2.0f);
		particleEmitter3->SetMinEnergy(1.0f);
		particleEmitter3->SetMaxEnergy(2.0f);
		particleEmitter3->SetMinSizeGrow(3.5f);
		particleEmitter3->SetMaxSizeGrow(5.5f);
		particleEmitter3->SetMinEmitterRange(0.2f);
		particleEmitter3->SetMaxEmitterRange(0.5f);
		particleObj3->AddComponent(particleEmitter3);	
		particleObj3->GetTransform()->Translate(-123,-54,149);
		campfires->AddChild(particleObj3);

	// 9. Mill
	// a. Body
	auto millMaterial = new DiffuseMaterial();
	millMaterial->SetDiffuseTexture(L"./Resources/Textures/MillBody.jpg");
	gameContext.pMaterialManager->AddMaterial(millMaterial, 8);

	ModelComponent* millBodyModel = new ModelComponent(L"./Resources/Meshes/MillBody.ovm");
	millBodyModel->SetMaterial(8);

	GameObject* millBody = new GameObject();
	millBody->AddComponent(millBodyModel);

	auto millBodyRigidBody = new RigidBodyComponent(true);
	millBody->AddComponent(millBodyRigidBody);
	
	auto millBodyMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/MillBody.ovpc");		

	shared_ptr<PxGeometry> millBodyGeometry(new PxConvexMeshGeometry(millBodyMesh));
	auto millBodyCollider = new ColliderComponent(millBodyGeometry,*standardMaterial);
	millBody->AddComponent(millBodyCollider);
	
	AddChild(millBody);
	// b. Propellers 
	m_pMillPropellersModel = new ModelComponent(L"./Resources/Meshes/MillPropellers.ovm");
	m_pMillPropellersModel->SetMaterial(8);

	GameObject* millPropellers = new GameObject();
	millPropellers->AddComponent(m_pMillPropellersModel);

	millPropellers->GetTransform()->Translate(68,19,103);
	
	AddChild(millPropellers);

	// 10. Rock
	auto rockMaterial = new DiffuseMaterial();
	rockMaterial->SetDiffuseTexture(L"./Resources/Textures/Rock.jpg");
	gameContext.pMaterialManager->AddMaterial(rockMaterial, 9);

	ModelComponent* rockModel = new ModelComponent(L"./Resources/Meshes/Rock.ovm");
	rockModel->SetMaterial(9);

	GameObject* rock = new GameObject();
	rock->AddComponent(rockModel);

	auto rockRigidBody = new RigidBodyComponent(true);
	rock->AddComponent(rockRigidBody);
	
	auto rockMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Rock.ovpt");		

	shared_ptr<PxGeometry> rockGeometry(new PxTriangleMeshGeometry(rockMesh));
	auto rockCollider = new ColliderComponent(rockGeometry,*standardMaterial);
	rock->AddComponent(rockCollider);
	
	AddChild(rock);

	// 11. Ruins
	ModelComponent* ruinsModel = new ModelComponent(L"./Resources/Meshes/Ruins.ovm");
	ruinsModel->SetMaterial(9);

	GameObject* ruins = new GameObject();
	ruins->AddComponent(ruinsModel);

	auto ruinsRigidBody = new RigidBodyComponent(true);
	ruins->AddComponent(ruinsRigidBody);
	
	auto ruinsMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Ruins.ovpt");		

	shared_ptr<PxGeometry> ruinsGeometry(new PxTriangleMeshGeometry(ruinsMesh));
	auto ruinsCollider = new ColliderComponent(ruinsGeometry,*standardMaterial);
	ruins->AddComponent(ruinsCollider);
	
	AddChild(ruins);

	// 12. Wood fences
	auto woodFencesMaterial = new DiffuseMaterial();
	woodFencesMaterial->SetDiffuseTexture(L"./Resources/Textures/WoodFences.jpg");
	gameContext.pMaterialManager->AddMaterial(woodFencesMaterial, 10);

	ModelComponent* woodFencesModel = new ModelComponent(L"./Resources/Meshes/WoodFences.ovm");
	woodFencesModel->SetMaterial(10);

	GameObject* woodFences = new GameObject();
	woodFences->AddComponent(woodFencesModel);

	auto woodFencesRigidBody = new RigidBodyComponent(true);
	woodFences->AddComponent(woodFencesRigidBody);
	
	auto woodFencesMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/WoodFences.ovpt");		

	shared_ptr<PxGeometry> woodFencesGeometry(new PxTriangleMeshGeometry(woodFencesMesh));
	auto woodFencesCollider = new ColliderComponent(woodFencesGeometry,*standardMaterial);
	woodFences->AddComponent(woodFencesCollider);
	
	AddChild(woodFences);
}

void Environment::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_Counter += gameContext.pGameTime->GetElapsed();	
	m_pMillPropellersModel->GetTransform()->Rotate(0, 45, 40 * m_Counter);
}


