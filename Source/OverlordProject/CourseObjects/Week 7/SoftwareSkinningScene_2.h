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
#include "Scenegraph/GameScene.h"
#include "Components/MeshDrawComponent.h"
#include "Helpers\EffectHelper.h"
#include "Skinnedvertex.h"
#include "BoneObject.h"

class Material;

class SoftwareSkinningScene_2: public GameScene
{
public:
	SoftwareSkinningScene_2(void);
	virtual ~SoftwareSkinningScene_2(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	BoneObject *m_pBone0, *m_pBone1; 
	float m_BoneRotation; 
	int m_RotationSign;

	void CreateMesh(); 
	MeshDrawComponent* m_pMeshDrawer;
	vector<SkinnedVertex> m_SkinnedVertices;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoftwareSkinningScene_2( const SoftwareSkinningScene_2 &obj);
	SoftwareSkinningScene_2& operator=( const SoftwareSkinningScene_2& obj);
};