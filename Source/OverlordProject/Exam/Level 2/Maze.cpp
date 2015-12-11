#include "Base/stdafx.h"
#include "Maze.h"

#include "../../../OverlordEngine/Physx/PhysxManager.h"
#include "../../../OverlordEngine/Components/ModelComponent.h"
#include "../../../OverlordEngine/Components/RigidBodyComponent.h"
#include "../../../OverlordEngine/Components/ColliderComponent.h"
#include "../../../OverlordEngine/Content/ContentManager.h"

Maze::Maze()
{
}

Maze::~Maze(void)
{
}

void Maze::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto standardMaterial = physX->createMaterial(0, 0, 0);

	// 1. Floor
	ModelComponent* mazeFloorModel = new ModelComponent(L"./Resources/Meshes/MFloor.ovm");
	mazeFloorModel->SetMaterial(59);

	GameObject* mazeFloor = new GameObject();
	mazeFloor->AddComponent(mazeFloorModel);

	auto mazeFloorRigidBody = new RigidBodyComponent(true);
	mazeFloor->AddComponent(mazeFloorRigidBody);
	
	auto mazeFloorMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/MFloor.ovpt");		

	shared_ptr<PxGeometry> mazeFloorGeom(new PxTriangleMeshGeometry(mazeFloorMesh));
	auto mazeFloorCollider = new ColliderComponent(mazeFloorGeom,*standardMaterial);
	mazeFloor->AddComponent(mazeFloorCollider);

	AddChild(mazeFloor);
	
	// 2. Walls
	ModelComponent* mazeWallsModel = new ModelComponent(L"./Resources/Meshes/MWalls.ovm");
	mazeWallsModel->SetMaterial(61);

	GameObject* mazeWalls = new GameObject();
	mazeWalls->AddComponent(mazeWallsModel);

	auto mazeWallsRigidBody = new RigidBodyComponent(true);
	mazeWalls->AddComponent(mazeWallsRigidBody);
	
	auto mazeWallsMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/MWalls.ovpt");		

	shared_ptr<PxGeometry> mazeWallsGeom(new PxTriangleMeshGeometry(mazeWallsMesh));
	auto mazeWallsCollider = new ColliderComponent(mazeWallsGeom,*standardMaterial);
	mazeWalls->AddComponent(mazeWallsCollider);

	AddChild(mazeWalls);
	
	// 3. Pillars
	ModelComponent* mazePillarsModel = new ModelComponent(L"./Resources/Meshes/MPillars.ovm");
	mazePillarsModel->SetMaterial(62);

	GameObject* mazePillars = new GameObject();
	mazePillars->AddComponent(mazePillarsModel);

	auto mazePillarsRigidBody = new RigidBodyComponent(true);
	mazePillars->AddComponent(mazePillarsRigidBody);
	
	auto mazePillarsMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/MPillars.ovpt");		

	shared_ptr<PxGeometry> mazePillarsGeom(new PxTriangleMeshGeometry(mazePillarsMesh));
	auto mazePillarsCollider = new ColliderComponent(mazePillarsGeom,*standardMaterial);
	mazePillars->AddComponent(mazePillarsCollider);
	
	AddChild(mazePillars);
	
	// 4. Tops
	ModelComponent* mazeTopsModel = new ModelComponent(L"./Resources/Meshes/MTops.ovm");
	mazeTopsModel->SetMaterial(63);

	GameObject* mazeTops = new GameObject();
	mazeTops->AddComponent(mazeTopsModel);
	
	auto mazeTopsRigidBody = new RigidBodyComponent(true);
	mazeTops->AddComponent(mazeTopsRigidBody);
	
	auto mazeTopsMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/MTops.ovpt");		
	
	shared_ptr<PxGeometry> mazeTopsGeom(new PxTriangleMeshGeometry(mazeTopsMesh));
	auto mazeTopsCollider = new ColliderComponent(mazeTopsGeom,*standardMaterial);
	mazeTops->AddComponent(mazeTopsCollider);
	
	AddChild(mazeTops);
}

void Maze::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}


