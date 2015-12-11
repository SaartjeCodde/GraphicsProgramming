#pragma once
#include "Scenegraph/GameScene.h"

class SkyboxScene : public GameScene
{
public:
	SkyboxScene(void);
	virtual ~SkyboxScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:	

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyboxScene( const SkyboxScene &obj);
	SkyboxScene& operator=( const SkyboxScene& obj);
};


