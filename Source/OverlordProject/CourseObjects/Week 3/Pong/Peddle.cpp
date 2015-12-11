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

#include "Peddle.h"
#include "Components\Components.h"
#include "Physx/PhysxManager.h"

Peddle::Peddle(float width, float height, float depth, XMFLOAT4 color):
	m_Dimensions(width,height,depth),
	m_Color(color)
{
}


Peddle::~Peddle(void)
{
}

void Peddle::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto defaultMaterial = physX->createMaterial(0, 0, 1);

	//Create Cube
	auto mesh = new MeshDrawComponent(12);
	
	float halfWidth = m_Dimensions.x/2.f;
	float halfHeight = m_Dimensions.y/2.f;
	float halfDepth = m_Dimensions.z/2.f;

	//FRONT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color)
		);

	//BACK
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),	
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color)	
		);

	//LEFT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(-1, 0, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(-1, 0, 0), m_Color)	
		);

	//RIGHT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(1, 0, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(1, 0, 0), m_Color)	
		);

	//TOP
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 1, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 1, 0), m_Color)	
		);

	//BOTTOM
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, -1, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, -1, 0), m_Color)	
		);

	AddComponent(mesh);


	auto rigidbody = new RigidBodyComponent();
	rigidbody->SetKinematic(true);
	AddComponent(rigidbody);

	std::shared_ptr<PxGeometry> geom(new PxBoxGeometry(halfWidth, halfHeight, halfDepth));
	AddComponent(new ColliderComponent(geom,*defaultMaterial));
}
