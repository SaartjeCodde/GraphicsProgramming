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

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"

#include "CourseObjects/Week 3/Pong/PongScene.h"
#include "CourseObjects/Week 3/Pong/TutorialScene.h"

#include "CourseObjects/Week 4/ModelTestScene.h"
#include "CourseObjects/Week 4/CharacterTest.h"

#include "CourseObjects/Week 5/SpikeyScene.h"
#include "CourseObjects/Week 5/SpriteTestScene.h"

#include "CourseObjects/Week 6/PickScene.h"
#include "CourseObjects/Week 6/SpriteFontTestScene.h"

#include "CourseObjects/Week 7/SoftwareSkinningScene_1.h"
#include "CourseObjects/Week 7/SoftwareSkinningScene_2.h"
#include "CourseObjects/Week 7/SoftwareSkinningScene_3.h"

#include "CourseObjects/Week 8/HardwareSkinningScene.h"

#include "CourseObjects/Week 9/PostProcessingScene.h"

#include "CourseObjects/Week 10/ParticleScene.h"

#include "CourseObjects/Week 11/SkyboxScene.h"

//#include "Exam/StartScreen.h"
//#include "Exam/Level 1/FirstLevelScene.h"
//#include "Exam/Level 2/SecondLevelScene.h"

MainGame::MainGame(void)
{
}

MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{
	//SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
//	SceneManager::GetInstance()->SetActiveGameScene(L"SpikeyScene");
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	/*SceneManager::GetInstance()->AddGameScene(new TutorialScene());
	SceneManager::GetInstance()->AddGameScene(new PongScene());
	
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTest());
	SceneManager::GetInstance()->AddGameScene(new PickScene());
	SceneManager::GetInstance()->AddGameScene(new SpriteFontTestScene());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());

	 SceneManager::GetInstance()->SetActiveGameScene(L"SoftwareSkinningScene_3");
	 SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
	 SceneManager::GetInstance()->SetActiveGameScene(L"HardwareSkinningScene");
	 SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
	 SceneManager::GetInstance()->SetActiveGameScene(L"PostProcessingScene");
	 SceneManager::GetInstance()->AddGameScene(new ParticleScene());
	 SceneManager::GetInstance()->SetActiveGameScene(L"ParticleScene");
	 SceneManager::GetInstance()->AddGameScene(new SkyboxScene());
	 SceneManager::GetInstance()->SetActiveGameScene(L"SkyboxScene");*/

	// EXAM
	//SceneManager::GetInstance()->AddGameScene(new StartScreen());
	//SceneManager::GetInstance()->SetActiveGameScene(L"StartScreen");
	//SceneManager::GetInstance()->AddGameScene(new FirstLevelScene());
	//SceneManager::GetInstance()->AddGameScene(new SecondLevelScene());
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
		}
	}

	return -1;
}
