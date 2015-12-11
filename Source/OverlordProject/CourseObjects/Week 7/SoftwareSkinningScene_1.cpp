#include "Base\stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Components/ModelComponent.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/TransformComponent.h"
#include "BoneObject.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1(void):	
	GameScene(L"SoftwareSkinningScene_1"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_BoneRotation(0),
	m_RotationSign(1)
{
}

SoftwareSkinningScene_1::~SoftwareSkinningScene_1(void)
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto boneMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(boneMaterial, 60);

	auto boneModel = new ModelComponent(L"./Resources/Meshes/Bone.ovm");
	boneModel->SetMaterial(0);
	auto bone = new GameObject();
	bone->AddComponent(boneModel);

	AddChild(bone);

	m_pBone0 = new BoneObject(0, 60, 15);
	m_pBone1 = new BoneObject(1, 60, 15);

	m_pBone0->AddBone(m_pBone1);
	auto root = new GameObject();
	root->AddChild(m_pBone0);
	AddChild(root);
	root->GetTransform()->Rotate(0, -90, 0);	
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	float elapsedSeconds = gameContext.pGameTime->GetElapsed();
	m_BoneRotation += m_RotationSign * 45 * elapsedSeconds;

	if (m_BoneRotation >= 45 && m_RotationSign > 0)
	{
		m_RotationSign = -1;
	}
	else if (m_BoneRotation <= -45 && m_RotationSign < 0)
	{
		m_RotationSign = 1;
	}

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0, 0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2, 0, 0);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
