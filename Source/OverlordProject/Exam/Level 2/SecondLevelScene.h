#pragma once
#include "Scenegraph/GameScene.h"
#include "../../../OverlordEngine/Graphics/SpriteFont.h"

class Character;
class ModelComponent;
class Maze;
class PointLightMaterial;
class Coin;
class PostGrayScale;

class SecondLevelScene: public GameScene
{
public:
	SecondLevelScene(void);
	~SecondLevelScene(void);

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	Character *m_CharacterController;
	ModelComponent* m_pKnightModel;
	Maze *m_pMaze;
	PointLightMaterial *m_pWallMaterial;
	PointLightMaterial *m_pTopMaterial;
	PointLightMaterial *m_pFloorMaterial;
	PointLightMaterial *m_pPillarMaterial;
	int m_AmountCollected;
	SpriteFont *m_pFont;
	static const int NUMBER_OF_COINS = 26; 
	Coin *m_pCoinArr[NUMBER_OF_COINS];
	PostGrayScale *m_Grey;
	bool m_DebugRenderingEnabled;
	
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SecondLevelScene(const SecondLevelScene &obj);
	SecondLevelScene& operator=(const SecondLevelScene &obj);
};