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
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"

#include "Ball.h"
#include "Components\Components.h"
#include "Physx/PhysxManager.h"

Ball::Ball(float size, XMFLOAT4 color)
	:
	m_Color(color),
	m_Size(size)
{
}


Ball::~Ball(void)
{
}

void Ball::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	// bouncy
	auto defaultMaterial = physX->createMaterial(0, 0, 1);


	auto rigidbody = new RigidBodyComponent();
	AddComponent(rigidbody);

	std::shared_ptr<PxGeometry> geom(new PxSphereGeometry(m_Size));
	AddComponent(new ColliderComponent(geom,*defaultMaterial));
}
