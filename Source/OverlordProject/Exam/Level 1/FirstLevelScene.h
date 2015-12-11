#pragma once
#include "Scenegraph/GameScene.h"
#include "../../../OverlordEngine/Graphics/SpriteFont.h"

class Character;
class ModelComponent;
class Environment;
class Key;

class FirstLevelScene: public GameScene
{
public:
	FirstLevelScene(void);
	~FirstLevelScene(void);

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	
	Character *m_CharacterController;
	ModelComponent* m_pKnightModel;
	Environment *m_pEnvironment;
	Key *m_pKey;
	bool m_KeyCollected;
	SpriteFont *m_pFont;
	float m_Counter;
	bool m_StartTimer;
	bool m_DebugRenderingEnabled;
	
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FirstLevelScene(const FirstLevelScene &obj);
	FirstLevelScene& operator=(const FirstLevelScene &obj);
};
