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
#include "stdafx.h"

#include "SpriteComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"
#include "../Graphics/TextureData.h"
#include "../Content/ContentManager.h"
#include "../Graphics/SpriteRenderer.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"

SpriteComponent::SpriteComponent(const wstring& spriteAsset, XMFLOAT2 pivot, XMFLOAT4 color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_pTexture(nullptr)
{
}

SpriteComponent::~SpriteComponent(void)
{
}

void SpriteComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SpriteComponent::Draw(const GameContext& gameContext)
{	
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_pTexture)
		return;

	// Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	// you will need to position, the rotation and the scale
	// You can use the QuaternionToEuler function to help you with the z rotation 
	
	XMFLOAT2 position;
	position.x = GetTransform()->GetPosition().x;
	position.y = GetTransform()->GetPosition().y;

	XMFLOAT2 scale;
	scale.x = GetTransform()->GetScale().x;
	scale.y = GetTransform()->GetScale().y;

	float rotation = QuaternionToEuler(GetTransform()->GetRotation()).z;
	float depth = GetTransform()->GetPosition().z;

	SpriteRenderer::GetInstance()->Draw(m_pTexture, position, m_Color, m_Pivot, scale, rotation, depth);
}
