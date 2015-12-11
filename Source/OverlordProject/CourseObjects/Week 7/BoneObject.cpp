#include "Base\stdafx.h"
#include "BoneObject.h"
#include "Components/BaseComponent.h"
#include "Components/ModelComponent.h"
#include "Components\TransformComponent.h"

#include "Scenegraph/GameObject.h"
#include "Scenegraph/GameScene.h"
#include "Base/OverlordGame.h"

BoneObject::BoneObject(int boneId, int materialId, float length):
	m_BoneId(boneId),
	m_MaterialId(materialId),
	m_Length(length)
{
}

BoneObject::~BoneObject(void)
{
}

void BoneObject::Initialize(const GameContext& gameContext)
{	
	UNREFERENCED_PARAMETER(gameContext);

	auto model = new ModelComponent(L"./Resources/Meshes/Bone.ovm");
	model->SetMaterial(m_MaterialId);

	auto object = new GameObject();
	object->AddComponent(model);
	
	AddChild(object);
	
	object->GetTransform()->Scale(m_Length, m_Length, m_Length);
}

void BoneObject::AddBone(BoneObject* pBone)
{	
	pBone->GetTransform()->Translate(0, 0, -m_Length);
	AddChild(pBone);
}

XMFLOAT4X4 BoneObject::GetBindPose()
{
	return m_BindPose;
}

void BoneObject::CalculateBindPose()
{
	XMFLOAT4X4 worldMatrix = GetTransform()->GetWorld();
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX invWorld = XMMatrixInverse(nullptr, world);
	XMStoreFloat4x4(&m_BindPose, invWorld);

	if (GetChild<BoneObject>() != nullptr)
	{
		GetChild<BoneObject>()->CalculateBindPose();
	}
}