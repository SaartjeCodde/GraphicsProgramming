#pragma once
#include "Scenegraph/GameScene.h"
class PostBlur;
class ModelComponent;

class Environment; 

class StartScreen : public GameScene
{
public:
	StartScreen(void);
	virtual ~StartScreen(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pButton;
	PostBlur *m_Blur;

	GameObject *m_pMill;
	ModelComponent* m_pMillPropellersModel;

		float m_Counter;

	Environment *m_pEnvironment;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	StartScreen(const StartScreen &obj);
	StartScreen& operator=(const StartScreen& obj);
};

