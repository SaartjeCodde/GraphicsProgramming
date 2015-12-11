//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.81
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameScene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "../Diagnostics/Logger.h"
#include "../Prefabs/Prefabs.h"
#include "../Components/Components.h"
#include "../Diagnostics/DebugRenderer.h"
#include "../Graphics/SpriteRenderer.h"
#include "../Graphics/TextRenderer.h"
#include "../Physx/PhysxProxy.h"
#include "../Graphics/PostProcessingMaterial.h"
#include "OverlordGame.h"
#include "../Graphics/RenderTarget.h"


GameScene::GameScene(wstring sceneName):
	m_pChildren(vector<GameObject*>()),
	m_pPostProcesses(vector<PostProcessingMaterial*>()),
	m_IsInitialized(false),
	m_GameContext(GameContext()),
	m_SceneName(sceneName),
	m_pDefaultCamera(nullptr),
	m_pActiveCamera(nullptr),
	m_pPhysxProxy(nullptr)
{
}


GameScene::~GameScene(void)
{
	SafeDelete(m_GameContext.pGameTime);
	SafeDelete(m_GameContext.pInput);
	SafeDelete(m_GameContext.pMaterialManager);

	for(GameObject* pChild:m_pChildren)
	{
		SafeDelete(pChild);
	}

	for(PostProcessingMaterial* pProces : m_pPostProcesses) SafeDelete(pProces);

	SafeDelete(m_pPhysxProxy);
}

void GameScene::AddChild(GameObject* obj)
{
#if _DEBUG
	if(obj->m_pParentScene)
	{
		if(obj->m_pParentScene == this)
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to this GameScene");
		else
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to another GameScene. Detach it from it's current scene before attaching it to another one.");

		return;
	}

	if(obj->m_pParentObject)
	{
		Logger::LogWarning(L"GameScene::AddChild > GameObject is currently attached to a GameObject. Detach it from it's current parent before attaching it to another one.");
		return;
	}
#endif

	obj->m_pParentScene = this;
	obj->RootInitialize(m_GameContext);
	m_pChildren.push_back(obj);
}

void GameScene::RemoveChild(GameObject* obj, bool deleteObject)
{
	auto it = find(m_pChildren.begin(), m_pChildren.end(), obj);

#if _DEBUG
	if(it == m_pChildren.end())
	{
		Logger::LogWarning(L"GameScene::RemoveChild > GameObject to remove is not attached to this GameScene!");
		return;
	}
#endif

	m_pChildren.erase(it);
	if(deleteObject)
	{
		delete obj;
		obj = nullptr;
	}
	else
		obj->m_pParentScene = nullptr;
}

void GameScene::RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if(m_IsInitialized)
		return;

	//Create DefaultCamera
	auto freeCam = new FreeCamera();
	freeCam->SetRotation(30, 0);
	freeCam->GetTransform()->Translate(0,50,-80);
	AddChild(freeCam);
	m_pDefaultCamera = freeCam->GetComponent<CameraComponent>();
	m_pActiveCamera = m_pDefaultCamera;
	m_GameContext.pCamera = m_pDefaultCamera;

	//Create GameContext
	m_GameContext.pGameTime = new GameTime();
	m_GameContext.pGameTime->Reset();
	m_GameContext.pGameTime->Stop();

	m_GameContext.pInput = new InputManager();
	m_GameContext.pInput->Initialize();

	m_GameContext.pMaterialManager = new MaterialManager();

	m_GameContext.pDevice = pDevice;
	m_GameContext.pDeviceContext = pDeviceContext;

	// Initialize Physx
	m_pPhysxProxy = new PhysxProxy();
	m_pPhysxProxy->Initialize(this);

	//User-Scene Initialize
	Initialize(m_GameContext);

	//Root-Scene Initialize
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootInitialize(m_GameContext);
	}

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_GameContext.pGameTime->Update();
	m_GameContext.pInput->Update();
	m_GameContext.pCamera = m_pActiveCamera;

	//User-Scene Update
	Update(m_GameContext);

	//Root-Scene Update
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootUpdate(m_GameContext);
	}

	//Post update
	/*for(GameObject* pChild:m_pChildren)
	{
		pChild->RootPostUpdate(m_GameContext);
	}*/

	m_pPhysxProxy->Update(m_GameContext);

	//Sort PostProcessingVector
	if(m_pPostProcesses.size() > 1)
	{
		for(unsigned int i = 1; i < m_pPostProcesses.size(); ++i)
		{
			if(m_pPostProcesses.at(i)->GetRenderIndex() < m_pPostProcesses.at(i-1)->GetRenderIndex() ) 
			{
				PostProcessingMaterial *temp = m_pPostProcesses.at(i);
				m_pPostProcesses.at(i) = m_pPostProcesses.at(i - 1);
				m_pPostProcesses.at(i - 1) = temp;
			}
		}
	}
}

void GameScene::RootDraw()
{
	//User-Scene Draw
	Draw(m_GameContext);

	//Object-Scene Draw
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootDraw(m_GameContext);
	}

	//Object-Scene Post-Draw
	for (GameObject* pChild : m_pChildren)
	{
		pChild->RootPostDraw(m_GameContext);
	}

	//Draw PhysX
	m_pPhysxProxy->Draw(m_GameContext);

	//Draw Debug Stuff
	DebugRenderer::Draw(m_GameContext);
	SpriteRenderer::GetInstance()->Draw(m_GameContext);
	TextRenderer::GetInstance()->Draw(m_GameContext);
	
	//Your own PP implementation :)
	RenderTarget *target = SceneManager::GetInstance()->GetGame()->GetRenderTarget();
	RenderTarget *prevTarget = target;

	for(unsigned int i = 0; i < m_pPostProcesses.size(); ++i)
	{
		SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pPostProcesses.at(i)->GetRenderTarget() );
		
		m_pPostProcesses.at(i)->Draw(m_GameContext, prevTarget); 
		prevTarget = m_pPostProcesses.at(i)->GetRenderTarget();
	}
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(target);

	//target = SceneManager::GetInstance()->GetGame()->GetRenderTarget();
	SpriteRenderer::GetInstance()->DrawImmediate(m_GameContext, prevTarget->GetShaderResourceView(), XMFLOAT2(0,0));
}

void GameScene::RootSceneActivated()
{
	//Start Timer
	m_GameContext.pGameTime->Start();
	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Stop Timer
	m_GameContext.pGameTime->Stop();
	SceneDeactivated();
}

void GameScene::SetActiveCamera(CameraComponent* pCameraComponent)
{ 
	//nullptr for DefaultCamera

	if(m_pActiveCamera!=nullptr)
		m_pActiveCamera->m_IsActive = false;

	m_pActiveCamera = (pCameraComponent)?pCameraComponent:m_pDefaultCamera;
	m_pActiveCamera->m_IsActive = true;
}

void GameScene::AddPostProcessingEffect(PostProcessingMaterial *effect)
{
	m_pPostProcesses.push_back(effect);
}

void GameScene::RemovePostProcessingEffect(PostProcessingMaterial *effect)
{
	vector<PostProcessingMaterial*>::iterator iter = m_pPostProcesses.begin();
	for(unsigned int i = 0; i < m_pPostProcesses.size(); ++i)
	{
		if(m_pPostProcesses.at(i) == effect)
			break;
		else
			++iter;
	}

	m_pPostProcesses.erase(iter);
}

