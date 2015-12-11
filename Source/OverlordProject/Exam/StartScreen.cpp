#include "Base\stdafx.h"
#include "StartScreen.h"

#include "../../OverlordEngine/Physx/PhysxManager.h"
#include "../../OverlordEngine/Prefabs/SkyBoxPrefab.h"
#include "../../OverlordEngine/Prefabs/CubePrefab.h"
#include "../../OverlordEngine/Components/TransformComponent.h"
#include "../../OverlordEngine/Components/RigidBodyComponent.h"
#include "../../OverlordEngine/Components/ColliderComponent.h"
#include "../../OverlordEngine/Components/SpriteComponent.h"
#include "../../OverlordEngine/Scenegraph/SceneManager.h"
#include "Level 1/Environment.h"
#include "../Materials/PostBlur.h"

StartScreen::StartScreen(void):
	GameScene(L"StartMenu")
{
}

StartScreen::~StartScreen(void)
{
}

void StartScreen::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto skyBox = new SkyBoxPrefab(L"./Resources/Textures/SkyBox.dds");
	AddChild(skyBox);	

	auto standardMaterial = physX->createMaterial(0, 0, 0);

	gameContext.pCamera->GetTransform()->Translate(0,130,-20);
	
	// *** Environment ***
	m_pEnvironment = new Environment();
	AddChild(m_pEnvironment);

	// *** PostProcessing ***
	m_Blur = new PostBlur();
	m_Blur->Initialize(gameContext);
	AddPostProcessingEffect(m_Blur);

	// *** Button ***
	auto button = new GameObject();
	button->AddComponent(new SpriteComponent(L"./Resources/Textures/StartButton.jpg",XMFLOAT2(-1.8f,-1.7f),XMFLOAT4(1,1,1,0.5f)));
	button->GetTransform()->Scale(0.15,0.15,0.15);
	AddChild(button);

	auto buttonPrefab = new CubePrefab();
	
	auto rigidBody = new RigidBodyComponent(true);
	buttonPrefab->AddComponent(rigidBody);

	std::shared_ptr<PxGeometry> boxGeom(new PxBoxGeometry(10,5,2));
	buttonPrefab->AddComponent(new ColliderComponent(boxGeom,*standardMaterial));
	buttonPrefab->GetTransform()->Translate(0,110,20);
	AddChild(buttonPrefab);	
}

void StartScreen::Update(const GameContext& gameContext)
{	
	// *** Picking ***
	if (gameContext.pInput->IsMouseButtonDown(VK_LBUTTON) )
	{
		GameObject* button = dynamic_cast<CubePrefab*>(gameContext.pCamera->Pick(gameContext));
		if (button != nullptr)
		{
			SceneManager::GetInstance()->NextScene();
		}
	}
}

void StartScreen::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
}
