#pragma once
#include "Scenegraph/GameObject.h"

class Key : public GameObject
{
public:
	Key(bool isTrigger = false);
	virtual ~Key();

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
	Key(const Key& t);
	Key& operator=(const Key& t);
};