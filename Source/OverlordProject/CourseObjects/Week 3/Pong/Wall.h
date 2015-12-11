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
#pragma once
#include "Scenegraph/GameObject.h"

class ColliderComponent;

class Wall : public GameObject
{
public:
	Wall(float width, float 
		, float depth, bool isTrigger = false);
	virtual ~Wall();

	virtual void Initialize(const GameContext& gameContext);
	
	bool IsTriggered() const { return m_WasTriggered; }
	void Reset(){ m_WasTriggered = false; }

private:

	XMFLOAT3 m_Dimensions;
	bool m_IsTrigger;
	bool m_WasTriggered;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Wall(const Wall& t);
	Wall& operator=(const Wall& t);
};

