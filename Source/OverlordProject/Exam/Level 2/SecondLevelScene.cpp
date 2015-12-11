#include "Base/stdafx.h"
#include "SecondLevelScene.h"

#include "../../CourseObjects/Week 4/Character.h"
#include "../../../OverlordEngine/Components/ModelComponent.h"
#include "../../../OverlordEngine/Components/TransformComponent.h"
#include "../../../OverlordEngine/Components/SpriteComponent.h"
#include "../../../OverlordEngine/Components/RigidBodyComponent.h"
#include "../../../OverlordEngine/Components/ColliderComponent.h"
#include "../../Materials/PointLightMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "../../Materials/SkinnedDiffuseMaterial.h"
#include "../../../OverlordEngine/Graphics/ModelAnimator.h"
#include "../../../OverlordEngine/Graphics/TextRenderer.h"
#include "../../../OverlordEngine/Content/ContentManager.h"
#include "../../../OverlordEngine/Prefabs/CubePrefab.h"
#include "../../../OverlordEngine/Prefabs/SkyBoxPrefab.h"
#include "../../../OverlordEngine/Physx/PhysxManager.h"
#include "../../../OverlordEngine/Scenegraph/SceneManager.h"
#include "../../Materials/PostgrayScale.h"
#include "../../../OverlordEngine/Physx/PhysxProxy.h"
#include "Maze.h"
#include "Coin.h"

SecondLevelScene::SecondLevelScene(void):
	GameScene(L"SecondLevelScene"),
	m_CharacterController(nullptr),
	m_pKnightModel(nullptr),
	m_pMaze(nullptr),
	m_pWallMaterial(nullptr),
	m_pTopMaterial(nullptr),
	m_pFloorMaterial(nullptr),
	m_pPillarMaterial(nullptr),
	m_AmountCollected(0),
	m_pFont(nullptr),
	m_Grey(nullptr),
	m_DebugRenderingEnabled(false)

{
	for (int index = 0 ; index < NUMBER_OF_COINS ; index++)
	{
		m_pCoinArr[index] = nullptr;
	}
}

SecondLevelScene::~SecondLevelScene(void)
{
}

void SecondLevelScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto standardMaterial = physX->createMaterial(0, 0, 0);

	// *** Character controller ***
	m_CharacterController = new Character();
	m_CharacterController->GetTransform()->Translate(210,5,-210);
	AddChild(m_CharacterController);

	// *** Knight ***
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 16);
	
	m_pKnightModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pKnightModel->SetMaterial(16);		
	auto knight = new GameObject();
	knight->AddComponent(m_pKnightModel);
	
	AddChild(knight);		
	knight->GetTransform()->Scale(0.02f, 0.02f, 0.02f);
	
	// *** Materials ***
	m_pFloorMaterial = new PointLightMaterial();
	m_pFloorMaterial ->SetDiffuseTexture(L"./Resources/Textures/MazeFloor.jpg");
	gameContext.pMaterialManager->AddMaterial(m_pFloorMaterial, 59);

	m_pWallMaterial  = new PointLightMaterial();
	m_pWallMaterial ->SetDiffuseTexture(L"./Resources/Textures/MazeWall.jpg");
	gameContext.pMaterialManager->AddMaterial(m_pWallMaterial, 61);

	m_pPillarMaterial  = new PointLightMaterial();
	m_pPillarMaterial->SetDiffuseTexture(L"./Resources/Textures/MazeTop.jpg");
	gameContext.pMaterialManager->AddMaterial(m_pPillarMaterial, 62);

	m_pTopMaterial = new PointLightMaterial();
	m_pTopMaterial->SetDiffuseTexture(L"./Resources/Textures/MazeTop.jpg");
	gameContext.pMaterialManager->AddMaterial(m_pTopMaterial, 63);

	auto coinMaterial = new DiffuseMaterial();
	coinMaterial->SetDiffuseTexture(L"./Resources/Textures/Coin.jpg");
	gameContext.pMaterialManager->AddMaterial(coinMaterial, 15);

	// *** Maze ***
	m_pMaze = new Maze();
	AddChild(m_pMaze);

	// *** Skybox ***
	auto skyBox = new SkyBoxPrefab(L"./Resources/Textures/NightSky.dds");
	AddChild(skyBox);	

	// *** Font ***
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");	

	// *** Coins sprite ***
	auto coinsSprite = new GameObject();
	coinsSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/Coins.png",XMFLOAT2(0.0f,0.0f),XMFLOAT4(1,1,1,0.5f)));
	coinsSprite->GetTransform()->Scale(0.4,0.4,0.4);
	AddChild(coinsSprite);

	// *** Treasure chest ***
	auto treasureChestModel = new ModelComponent(L"./Resources/Meshes/TreasureChest.ovm");
	auto treasureChestMaterial = new DiffuseMaterial();
	treasureChestMaterial->SetDiffuseTexture(L"./Resources/Textures/TreasureChest.jpg"); 
	
	gameContext.pMaterialManager->AddMaterial(treasureChestMaterial, 111);
	treasureChestModel->SetMaterial(111);
	
	auto treasureChest = new GameObject();
	treasureChest->AddComponent(treasureChestModel);	
	treasureChest->GetTransform()->Translate(-222,2,198);
	treasureChest->GetTransform()->Scale(0.5,0.5,0.5);
	
	AddChild(treasureChest);	
	
	auto cubePrefabTreasureChest = new CubePrefab();

	auto rigidBody = new RigidBodyComponent(true);
	cubePrefabTreasureChest->AddComponent(rigidBody);

	std::shared_ptr<PxGeometry> boxGeom(new PxBoxGeometry(5,5,5));
	cubePrefabTreasureChest->AddComponent(new ColliderComponent(boxGeom,*standardMaterial));
	cubePrefabTreasureChest->GetTransform()->Translate(-222,2,198);	
	AddChild(cubePrefabTreasureChest);

	// *** Coins ***
	for (int index = 0 ; index < NUMBER_OF_COINS ; index++)
	{
		m_pCoinArr[index] = new Coin(true);
	}
	
	m_pCoinArr[0]->GetTransform()->Translate(21,3.5,-62);
	m_pCoinArr[1]->GetTransform()->Translate(22,3.5,-104);
	m_pCoinArr[2]->GetTransform()->Translate(105,3.5,-105);
	m_pCoinArr[3]->GetTransform()->Translate(148,3.5,-62);
	m_pCoinArr[4]->GetTransform()->Translate(232,3.5,-63);
	m_pCoinArr[5]->GetTransform()->Translate(-20,3.5,-229);
	m_pCoinArr[6]->GetTransform()->Translate(-62,3.5,-229);
	m_pCoinArr[7]->GetTransform()->Translate(-229,3.5,-189);
	m_pCoinArr[8]->GetTransform()->Translate(146,3.5,-147);
	m_pCoinArr[9]->GetTransform()->Translate(-229,3.5,146);
	m_pCoinArr[10]->GetTransform()->Translate(-145,3.5,105);
	m_pCoinArr[11]->GetTransform()->Translate(-145,3.5,188);
	m_pCoinArr[12]->GetTransform()->Translate(-60,3.5,231);
	m_pCoinArr[13]->GetTransform()->Translate(-20,3.5,146);
	m_pCoinArr[14]->GetTransform()->Translate(64,3.5,188);
	m_pCoinArr[15]->GetTransform()->Translate(189,3.5,189);
	m_pCoinArr[16]->GetTransform()->Translate(105,3.5,105);
	m_pCoinArr[17]->GetTransform()->Translate(229,3.5,62);
	m_pCoinArr[18]->GetTransform()->Translate(-187,3.5,63);
	m_pCoinArr[19]->GetTransform()->Translate(-102,3.5,17);
	m_pCoinArr[20]->GetTransform()->Translate(-105,3.5,-63);
	m_pCoinArr[21]->GetTransform()->Translate(-226,3.5,-63);
	m_pCoinArr[22]->GetTransform()->Translate(-20,3.5,19);
	m_pCoinArr[23]->GetTransform()->Translate(105,3.5,20);
	m_pCoinArr[24]->GetTransform()->Translate(105,3.5,-16);
	m_pCoinArr[25]->GetTransform()->Translate(64,3.5,-187);

	for (int index = 0 ; index < NUMBER_OF_COINS ; index++)
	{
		AddChild(m_pCoinArr[index]);
	}

	// *** PostProcessing ***
	m_Grey = new PostGrayScale();
	m_Grey->Initialize(gameContext);
	AddPostProcessingEffect(m_Grey);
}

void SecondLevelScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
	
	// *** Character position ***
	auto posCharacter = m_CharacterController->GetTransform()->GetPosition();

	// *** Teleport to chest ***
	if (gameContext.pInput->IsKeyboardKeyDown('T'))
	{
		m_CharacterController->GetTransform()->Translate(-200,5.0f,200.0f);
	}

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

	// *** Pointlight translation ***
	m_pWallMaterial->SetLightPos(posCharacter, 0);
	m_pTopMaterial->SetLightPos(posCharacter, 0);
	m_pFloorMaterial->SetLightPos(posCharacter, 0);
	m_pPillarMaterial->SetLightPos(posCharacter, 0);

	// *** Coin triggers ***
	for (int index = 0 ; index < NUMBER_OF_COINS ; index++)
	{
		if (m_pCoinArr[index] != nullptr)
		{
			if (m_pCoinArr[index]->IsTriggered())
			{
				RemoveChild(m_pCoinArr[index]);
				m_pCoinArr[index] = nullptr;
				m_AmountCollected++;
			}
		}
	} 

	// *** Picking ***
	if (gameContext.pInput->IsMouseButtonDown(VK_LBUTTON) && !gameContext.pInput->IsMouseButtonDown(VK_LBUTTON, true))
	{
		GameObject* treasureChest = dynamic_cast<CubePrefab*>(gameContext.pCamera->Pick(gameContext));
			
		if (treasureChest != nullptr)
		{			
			SceneManager::GetInstance()->NextScene();			
		}
	}

	// *** HUD ***
	std::wstring coins = std::to_wstring(m_AmountCollected);
	TextRenderer::GetInstance()->DrawText(m_pFont, L"collected: " + coins, XMFLOAT2(120,40), XMFLOAT4(1,0,0,1));	
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Find the treasure chest", XMFLOAT2(450,40), XMFLOAT4(1,0,0,1));	
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press 'T' to teleport to chest", XMFLOAT2(820,650), XMFLOAT4(1,0,0,1));

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

void SecondLevelScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

