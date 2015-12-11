#pragma once
#include "Scenegraph/GameObject.h"

class ModelComponent;

class Environment : public GameObject
{
public:
	Environment();
	virtual ~Environment();

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

private:	

	ModelComponent* m_pMillPropellersModel;
	float m_Counter;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Environment(const Environment& yRef);
	Environment& operator=(const Environment& yRef);
};
