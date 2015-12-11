//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.121
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings):
	m_VertexInfo(ParticleVertex()),
	m_EmitterSettings(emitterSettings),
	m_TotalEnergy(0),
	m_CurrentEnergy(0),
	m_SizeGrow(0),
	m_InitSize(0),
	m_IsActive(false)
{
}

Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive) return;

	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();

	if (m_CurrentEnergy < 0)
	{
		m_IsActive = false;
		return;
	}

	XMVECTOR velocity = XMLoadFloat3( &m_EmitterSettings.Velocity );
	XMVECTOR position = XMLoadFloat3( &m_VertexInfo.Position );
	position += velocity * gameContext.pGameTime->GetElapsed();
	XMStoreFloat3( &m_VertexInfo.Position, position );

	m_VertexInfo.Color = m_EmitterSettings.Color;

	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2;

	if(m_SizeGrow != 1)
	{
		m_VertexInfo.Size = m_InitSize + m_InitSize * ( ( m_SizeGrow - 1 ) * ( 1 - particleLifePercent ) );
	}
}

void Particle::Init(XMFLOAT3 initPosition)
{
	UNREFERENCED_PARAMETER(initPosition);
	
	m_IsActive = true;

	// Energy
	m_TotalEnergy = randF(m_EmitterSettings.MinEnergy,m_EmitterSettings.MaxEnergy); 
	m_CurrentEnergy = m_TotalEnergy;

	// SpawnPosition
	XMFLOAT3 randomDirection(1,0,0);
	XMVECTOR vRandomDirection = XMLoadFloat3(&randomDirection);
	XMMATRIX randomMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	XMVECTOR randNormalizedDirection = XMVector3TransformNormal(vRandomDirection, randomMatrix);
	float distance = randF( m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange );
	randNormalizedDirection *= distance;
	XMStoreFloat3( &randomDirection, randNormalizedDirection);
	m_VertexInfo = randomDirection;

	// Size
	m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_VertexInfo.Size = m_InitSize;

	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow); 

	// Rotation
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
