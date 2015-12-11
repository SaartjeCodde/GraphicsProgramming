//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.108
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CameraComponent.h"
#include "../Base/OverlordGame.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"

CameraComponent::CameraComponent(void):
	m_FOV(XM_PIDIV4),
	m_NearPlane(0.1f),
	m_FarPlane(2500.0f),
	m_Size(25.0f),
	m_PerspectiveProjection(true),
	m_IsActive(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view, viewInv, viewProjectionInv;

	if(m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio ,m_NearPlane, m_FarPlane);
	}
	else
	{
		float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	viewInv = XMMatrixInverse(nullptr, view);
	viewProjectionInv = XMMatrixInverse(nullptr, view * projection);
	
	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}
void CameraComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if(gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if(gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext) const
{
	// Step 1: get the GameObject, GameScene and PhysXProxy
	// be sure to include safety checks!!
	GameObject *gameObject;
	gameObject = GetGameObject();
	GameScene *gameScene;
	
	if (gameObject != nullptr)
	{
		gameScene = gameObject->GetScene();
	}
	else 
	{
		(MessageBox(NULL, L"GameObject in PickScene is NULL", L"Error", MB_OK));
	}

	PhysxProxy *physxProxy;
	if (gameScene != nullptr) 
	{
		physxProxy = gameScene->GetPhysxProxy();
	}
	else
	{
		MessageBox(NULL, L"GameScene in PickScene is NULL", L"Error", MB_OK);
	}

	// Step 2: created variable declaration for
	// * near point and far point (float3)
	XMFLOAT3 nearPoint, farPoint;
	// * NDC (float2)
	XMFLOAT2 NDC;
	// * ray start and ray direction (PxVec3)
	PxVec3 rayStart, rayDirection;
	// * halfwidth and halfheight (Hint: OverlordGame::GetGameSettings holds usefull data)
	float halfHeight = (float)OverlordGame::GetGameSettings().Window.Height / 2;
	float halfWidth = (float)OverlordGame::GetGameSettings().Window.Width / 2;
	// * mouse position (the game context holds usefull data)
	POINT mousePosition = gameContext.pInput->GetMousePosition();
	
	// Step 3: Calculate x and y ndc coordinates
	NDC.x = (mousePosition.x - halfWidth) / halfWidth;
	NDC.y = (halfHeight - mousePosition.y) / halfHeight;

	// Step 4: Calculate near point and far point using the NDC coordinates and the inverse of the viewProjection matrix
	XMFLOAT4 nearPointNDC = XMFLOAT4(NDC.x, NDC.y, 0, 0);
	XMFLOAT4 farPointNDC = XMFLOAT4(NDC.x, NDC.y, 1, 0);
	XMStoreFloat3(&nearPoint, XMVector3TransformCoord(XMLoadFloat4(&nearPointNDC), XMLoadFloat4x4(&gameContext.pCamera->GetViewProjectionInverse())));
	XMStoreFloat3(&farPoint, XMVector3TransformCoord(XMLoadFloat4(&farPointNDC), XMLoadFloat4x4(&gameContext.pCamera->GetViewProjectionInverse())));

	// Step 5: Calculate the ray start and the direction (hint: the direction can be calculated using near point and far point)
	rayStart = PxVec3(nearPoint.x, nearPoint.y, nearPoint.z);
	rayDirection = PxVec3(farPoint.x, farPoint.y, farPoint.z) - PxVec3(nearPoint.x, nearPoint.y, nearPoint.z);
	rayDirection.normalize();

	// Step 6: Use the PhysxProxy to do a raycast. 
	// If the raycast has a hit, return the GameObject associated with the actor
	// (Hint: the actor can be fount in hit.block. The userData field of the actor should be a Component)
	PxRaycastBuffer hit;
	if(physxProxy->Raycast(rayStart, rayDirection, PX_MAX_F32, hit))
	{
		if(hit.hasBlock){

			auto test = hit.block.actor->userData;

			if(test != nullptr){
				BaseComponent *gameObj =  (BaseComponent*)test;
				return gameObj->GetGameObject();
			}
			else return nullptr;

		}

	}

	return nullptr;
}
