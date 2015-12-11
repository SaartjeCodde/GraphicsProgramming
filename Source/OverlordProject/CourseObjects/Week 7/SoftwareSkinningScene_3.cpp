#include "Base\stdafx.h"
#include "Physx\PhysxManager.h"
#include "Physx\PhysxProxy.h"
#include "SoftwareSkinningScene_3.h"
#include "Components/ModelComponent.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/TransformComponent.h"
#include "BoneObject.h"

#define FPS_COUNTER 1

SoftwareSkinningScene_3::SoftwareSkinningScene_3(void):	
	GameScene(L"SoftwareSkinningScene_3"),
	m_pBone0(nullptr),
	m_pBone1(nullptr), 
	m_BoneRotation(0),
	m_RotationSign(1),
	m_pMeshDrawer(nullptr)
{
}

SoftwareSkinningScene_3::~SoftwareSkinningScene_3(void)
{
}

void SoftwareSkinningScene_3::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	AddChild(ground);

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

void SoftwareSkinningScene_3::Update(const GameContext& gameContext)
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
	auto transform1 = bindpos * world;
	
	for (int index = 0; index < m_SkinnedVertices.size(); ++index)
	{
		float blendWeight0 = m_SkinnedVertices[index].m_BlendWeight1;
		float blendWeight1 = m_SkinnedVertices[index].m_BlendWeight2;

		XMVECTOR pos = XMLoadFloat3(&m_SkinnedVertices[index].OriginalVertex.Position);
		pos = XMVector3TransformCoord(pos, transform0 * blendWeight0 + transform1 * blendWeight1);
		XMStoreFloat3(&m_SkinnedVertices[index].TransformedVertex.Position, pos);
	}
}

void SoftwareSkinningScene_3::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pMeshDrawer->RemoveTriangles();
	for (int i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		QuadPosNormCol quad;
		quad.Vertex1 = m_SkinnedVertices[i].TransformedVertex;
		quad.Vertex2 = m_SkinnedVertices[i+1].TransformedVertex;
		quad.Vertex3 = m_SkinnedVertices[i+2].TransformedVertex;
		quad.Vertex4 = m_SkinnedVertices[i+3].TransformedVertex;
	
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}


void SoftwareSkinningScene_3::CreateMesh() 
{
	auto pos = XMFLOAT3(7.5f, 0, 0); 
	auto offset = XMFLOAT3(7.5f, 2.5f, 2.5f); 
	auto col = XMFLOAT4(1, 0, 0, 0.5f);
	
	// ***** 
	// BOX1*
	// *****
	
	// FRONT 
	auto norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	
	// BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	
	// TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f)); 
	
	// BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	
	// LEFT 
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.f, 0.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.f, 0.f)); 
	
	// RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	
	// ***** 
	// BOX2* 
	// *****

	col = XMFLOAT4(0, 1, 0, 0.5f); 
	pos = XMFLOAT3(22.5f, 0, 0); 
	// FRONT 
	norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	// BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f)); 
	// TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	// BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	// LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f)); 
	// RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.f, 1.f)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f)); 
}