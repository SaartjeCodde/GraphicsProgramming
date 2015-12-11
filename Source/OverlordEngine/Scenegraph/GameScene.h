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
#pragma once
#include "../Base/GeneralStructs.h"

//Forward Declarations
class GameObject;
class SceneManager;
class CameraComponent;
class PhysxProxy;
class PostProcessingMaterial;

class GameScene
{
public:
	GameScene(wstring sceneName);
	virtual ~GameScene(void);

	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj, bool deleteObject = true);
	const GameContext& GetGameContext() const { return m_GameContext; }

	PhysxProxy * GetPhysxProxy() const { return m_pPhysxProxy; }
	void SetActiveCamera(CameraComponent* pCameraComponent);
	void AddPostProcessingEffect(PostProcessingMaterial *effect);
	void RemovePostProcessingEffect(PostProcessingMaterial *effect);

protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;
	virtual void SceneActivated(){}
	virtual void SceneDeactivated(){}

private:

	friend class SceneManager;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();

	vector<GameObject*> m_pChildren;
	vector<PostProcessingMaterial*> m_pPostProcesses;
	GameContext m_GameContext;
	bool m_IsInitialized;
	wstring m_SceneName;
	CameraComponent *m_pDefaultCamera, *m_pActiveCamera;
	PhysxProxy* m_pPhysxProxy;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameScene( const GameScene &obj);
	GameScene& operator=( const GameScene& obj);
};
