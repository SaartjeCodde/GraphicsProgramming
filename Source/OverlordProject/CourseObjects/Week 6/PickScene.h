#pragma once
#include "Scenegraph/GameScene.h"

class PickScene : public GameScene
{
public:
	PickScene(void);
	virtual ~PickScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PickScene( const PickScene &obj);
	PickScene& operator=( const PickScene& obj);
};
