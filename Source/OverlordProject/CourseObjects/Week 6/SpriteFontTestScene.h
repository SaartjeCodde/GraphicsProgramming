#pragma once
#include "Scenegraph/GameScene.h"
#include "../../../OverlordEngine/Graphics/SpriteFont.h"

class SpriteFontTestScene : public GameScene
{
public:
	SpriteFontTestScene(void);
	virtual ~SpriteFontTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	SpriteFont *m_pFont;
	
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteFontTestScene( const SpriteFontTestScene &obj);
	SpriteFontTestScene& operator=( const SpriteFontTestScene& obj);
};
