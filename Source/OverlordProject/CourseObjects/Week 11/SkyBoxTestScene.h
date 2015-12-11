#pragma once
#include "Scenegraph/GameScene.h"

class SkyBoxPrefab;

class SkyBoxTestScene : public GameScene
{
public:
	SkyBoxTestScene(void);
	virtual ~SkyBoxTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	SkyBoxPrefab *m_pSkyBox;
	
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxTestScene( const SkyBoxTestScene &obj);
	SkyBoxTestScene& operator=( const SkyBoxTestScene& obj);
};


