#pragma once
#include "Scenegraph/GameObject.h"

class Coin : public GameObject
{
public:
	Coin(bool isTrigger = false);
	virtual ~Coin();

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	
	bool IsTriggered() const { return m_WasTriggered; }
	void Reset();
	bool returnEndTrigger(){return m_EndTriggering;}

private:

	bool m_IsTrigger;
	bool m_WasTriggered;
	bool m_EndTriggering;	

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Coin(const Coin& t);
	Coin& operator=(const Coin& t);
};

