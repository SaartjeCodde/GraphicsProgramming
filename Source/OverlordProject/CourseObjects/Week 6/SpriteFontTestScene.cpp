#include "Base\stdafx.h"
#include "SpriteFontTestScene.h"
#include "Content/ContentManager.h"
#include "../../../OverlordEngine/Graphics/TextRenderer.h"

SpriteFontTestScene::SpriteFontTestScene(void):
	GameScene(L"SpriteFontTestScene"),
	m_pFont(nullptr)
{
}

SpriteFontTestScene::~SpriteFontTestScene(void)
{
}

void SpriteFontTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");	
}

void SpriteFontTestScene::Update(const GameContext& gameContext)
{	
}

void SpriteFontTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	

	TextRenderer::GetInstance()->DrawText(m_pFont, L"Hello World", XMFLOAT2(10, 10), XMFLOAT4(1,0,0,1));
}
