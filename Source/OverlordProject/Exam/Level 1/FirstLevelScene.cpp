#include "Base/stdafx.h"
#include "FirstLevelScene.h"

#include "../../CourseObjects/Week 4/Character.h"
#include "../../../OverlordEngine/Components/ModelComponent.h"
#include "../../../OverlordEngine/Components/TransformComponent.h"
#include "../../../OverlordEngine/Components/ColliderComponent.h"
#include "../../../OverlordEngine/Components/RigidBodyComponent.h"
#include "../../../OverlordEngine/Components/SpriteComponent.h"
#include "../../Materials/SkinnedDiffuseMaterial.h"
#include "../../../OverlordEngine/Graphics/ModelAnimator.h"
#include "../../../OverlordEngine/Prefabs/SkyBoxPrefab.h"
#include "../../../OverlordEngine/Prefabs/CubePrefab.h"
#include "../../../OverlordEngine/Content/ContentManager.h"
#include "../../../OverlordEngine/Graphics/TextRenderer.h"
#include "../../../OverlordEngine/Physx/PhysxManager.h"
#include "../../../OverlordEngine/Physx/PhysxProxy.h"
#include "../../../OverlordEngine/Scenegraph/SceneManager.h"
#include "Environment.h"
#include "Key.h"

FirstLevelScene::FirstLevelScene(void):
	GameScene(L"FirstLevelScene"),
	m_CharacterController(nullptr),
	m_pKnightModel(nullptr),
	m_pEnvironment(nullptr),
	m_pKey(nullptr),
	m_KeyCollected(false),
	m_pFont(nullptr),
	m_Counter(0.0f),
	m_StartTimer(false),
	m_DebugRenderingEnabled(false)
{
}

FirstLevelScene::~FirstLevelScene(void)
{
}

void FirstLevelScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
	
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto standardMaterial = physX->createMaterial(0, 0, 0);

	// *** Character controller ***
	m_CharacterController = new Character();
	m_CharacterController->GetTransform()->Translate(70,0,-120);
	AddChild(m_CharacterController);

	// *** Knight ***
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);
	
	m_pKnightModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pKnightModel->SetMaterial(0);		
	auto knight = new GameObject();
	knight->AddComponent(m_pKnightModel);
	
	AddChild(knight);		
	knight->GetTransform()->Scale(0.02f, 0.02f, 0.02f);

	// *** Skybox ***
	auto skyBox = new SkyBoxPrefab(L"./Resources/Textures/SkyBox.dds");
	AddChild(skyBox);

	// *** Environment ***
	m_pEnvironment = new Environment();
	AddChild(m_pEnvironment);

	// *** Key ***
	m_pKey = new Key(true);
	m_pKey->GetTransform()->Translate(140,-50,195);
	AddChild(m_pKey);

	// *** Font ***
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");	

	// *** Key sprite *** 
	auto keySprite = new GameObject();
	keySprite->AddComponent(new SpriteComponent(L"./Resources/Textures/Key.png",XMFLOAT2(-2.6f,0.0f),XMFLOAT4(1,1,1,0.5f)));
	keySprite->GetTransform()->Scale(0.3,0.3,0.3);
	AddChild(keySprite);

	// *** Cave entrance ***
	auto caveEntrance = new CubePrefab();

	auto rigidBody = new RigidBodyComponent(true);
	caveEntrance->AddComponent(rigidBody);

	std::shared_ptr<PxGeometry> boxGeom(new PxBoxGeometry(2,8,5));
	caveEntrance->AddComponent(new ColliderComponent(boxGeom,*standardMaterial));
	caveEntrance->GetTransform()->Translate(-3,-41,6);	
	AddChild(caveEntrance);
}

void FirstLevelScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	

	auto posCharacter = m_CharacterController->GetTransform()->GetPosition();

	// *** Knight model translation & rotation ***
	m_pKnightModel->GetTransform()->Translate(posCharacter.x, posCharacter.y - 4, posCharacter.z);
	m_pKnightModel->GetTransform()->Rotate(0,m_CharacterController->GetTotalYaw() - 180 , 0);

	// *** Knight animations ***
	auto animator = m_pKnightModel->GetAnimator();
	if (gameContext.pInput->IsKeyboardKeyDown('Z'))
	{
		animator->Play();
	}
	else 
	{
		animator->Reset();
		animator->Pause();
	}

	animator->SetAnimationSpeed(25);

	// *** Key trigger ***
	if (m_pKey != nullptr)
	{
		if (m_pKey->IsTriggered())
		{
			RemoveChild(m_pKey);
			m_pKey = nullptr;
			m_KeyCollected = true;
		}
	}

	// *** Picking ***
	if (gameContext.pInput->IsMouseButtonDown(VK_LBUTTON) )
	{
		GameObject* caveEntrance = dynamic_cast<CubePrefab*>(gameContext.pCamera->Pick(gameContext));
		if (caveEntrance)
		{
			if (m_KeyCollected == false)
			{
				m_StartTimer = true;
			}
			else
			{
				SceneManager::GetInstance()->NextScene();		
			}
		}
	}

	// *** HUD & related ***
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Inventory:", XMFLOAT2(10,10), XMFLOAT4(1,0,0,1));	
	if (m_KeyCollected)
	{
		TextRenderer::GetInstance()->DrawText(m_pFont, L"collected key", XMFLOAT2(220,10), XMFLOAT4(1,0,0,1));	
	}	
	else TextRenderer::GetInstance()->DrawText(m_pFont, L"not collected", XMFLOAT2(220,10), XMFLOAT4(1,0,0,1));	
	
	if (m_StartTimer)
	{
		m_Counter += gameContext.pGameTime->GetElapsed();
	}
	if (m_Counter > 1 && m_Counter < 3)
	{
		TextRenderer::GetInstance()->DrawText(m_pFont, L"You haven't found the key yet!", XMFLOAT2(400,150), XMFLOAT4(1,0,0,1));	
	}
	else if (m_Counter > 5)
	{
		m_StartTimer = false;
		m_Counter = 0;
	}		

	// *** Debug rendering ***
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press 'O' for debug rendering", XMFLOAT2(820,680), XMFLOAT4(1,0,0,1));
	if (gameContext.pInput->IsKeyboardKeyDown('O') && !gameContext.pInput->IsKeyboardKeyDown('O', true))
	{
		m_DebugRenderingEnabled = m_DebugRenderingEnabled ? false : true;
	}
	if (m_DebugRenderingEnabled == true)
	{
		GetPhysxProxy()->EnablePhysxDebugRendering(true);
	}
	else GetPhysxProxy()->EnablePhysxDebugRendering(false);
}

void FirstLevelScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

