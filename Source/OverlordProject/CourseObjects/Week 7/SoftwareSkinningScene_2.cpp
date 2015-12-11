#include "Base\stdafx.h"
#include "SoftwareSkinningScene_2.h"
#include "Components/ModelComponent.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/TransformComponent.h"
#include "BoneObject.h"

#define FPS_COUNTER 1

SoftwareSkinningScene_2::SoftwareSkinningScene_2(void):	
	GameScene(L"SoftwareSkinningScene_2"),
	m_pBone0(nullptr),
	m_pBone1(nullptr), 
	m_BoneRotation(0),
	m_RotationSign(1),
	m_pMeshDrawer(nullptr)
{
}

SoftwareSkinningScene_2::~SoftwareSkinningScene_2(void)
{
}

void SoftwareSkinningScene_2::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
		
	//GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//gameContext.pGameTime->ForceElapsedUpperbound(true);

	//// Create PhysX ground plane
	//auto physX = PhysxManager::GetInstance()->GetPhysics();
	//
	//auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	//auto ground = new GameObject();
	//ground->AddComponent(new RigidBodyComponent(true));
	//
	//std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	//ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	//AddChild(ground);

	auto colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);

	auto root = new GameObject();
	m_pBone0 = new BoneObject(0, 0, 15);
	m_pBone1 = new BoneObject(1, 0, 15);
	
	root->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);
	root->GetTransform()->Rotate(0,-90,0);
	AddChild(root);

	m_pBone0->CalculateBindPose();
	auto emptyObj = new GameObject();
	m_pMeshDrawer = new MeshDrawComponent(24);
	emptyObj->AddComponent(m_pMeshDrawer);
	AddChild(emptyObj);
	CreateMesh();	
}

void SoftwareSkinningScene_2::Update(const GameContext& gameContext)
{
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	
	if (m_BoneRotation < -45 && m_RotationSign < 0)
	{
		m_RotationSign = 1;
	}

	if (m_BoneRotation > 45 && m_RotationSign > 0)
	{
		m_RotationSign = -1;
	}

	m_pBone0->GetTransform()->Rotate(m_BoneRotation,0,0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation*2,0,0);
		
	auto world = XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	auto bindpos = XMLoadFloat4x4(&m_pBone0->GetBindPose());
	auto transform0 = bindpos * world;

	world = XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	bindpos = XMLoadFloat4x4(&m_pBone1->GetBindPose());
	auto transform1 =  bindpos * world;

	for (int i = 0; i < m_SkinnedVertices.size(); ++i)
	{
		if (i < 24)
		{
			XMVECTOR pos = XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
			pos = XMVector3TransformCoord(pos, transform0);
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, pos);
		}
		else
		{
			XMVECTOR pos = XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
			pos = XMVector3TransformCoord(pos, transform1);
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, pos);
		}
	}
}

void SoftwareSkinningScene_2::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
		
	m_pMeshDrawer->RemoveTriangles();
	for (int index = 0; index < m_SkinnedVertices.size(); index += 4)
	{
		QuadPosNormCol quad;
		quad.Vertex1 = m_SkinnedVertices[index].TransformedVertex;
		quad.Vertex2 = m_SkinnedVertices[index+1].TransformedVertex;
		quad.Vertex3 = m_SkinnedVertices[index+2].TransformedVertex;
		quad.Vertex4 = m_SkinnedVertices[index+3].TransformedVertex;
	
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::CreateMesh() 
{
	//auto pos = XMFLOAT3(7.5f, 0, 0); 
	//auto offset = XMFLOAT3(7.5f, 2.5f, 2.5f); 
	//auto col = XMFLOAT4(1, 0, 0, 0.5f);
	//// ***** 
	//// BOX1* 
	//// *****
	//// FRONT 
	//auto norm = XMFLOAT3(0, 0, -1); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	//
	//// BACK 
	//norm = XMFLOAT3(0, 0, 1); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//// TOP 
	//norm = XMFLOAT3(0, 1, 0); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//// BOTTOM 
	//norm = XMFLOAT3(0, -1, 0); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//// LEFT 
	//norm = XMFLOAT3(-1, 0, 0); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//// RIGHT 
	//norm = XMFLOAT3(1, 0, 0); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//// ***** 
	//// BOX2* 
	//// ***** 
	//col = XMFLOAT4(0, 1, 0, 0.5f); 
	//pos = XMFLOAT3(22.5f, 0, 0); 
	//// FRONT 
	//norm = XMFLOAT3(0, 0, -1); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//// BACK 
	//norm = XMFLOAT3(0, 0, 1); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//// TOP 
	//norm = XMFLOAT3(0, 1, 0); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//// BOTTOM 
	//norm = XMFLOAT3(0, -1, 0); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//// LEFT 
	//norm = XMFLOAT3(-1, 0, 0); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//// RIGHT 
	//norm = XMFLOAT3(1, 0, 0); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
}
