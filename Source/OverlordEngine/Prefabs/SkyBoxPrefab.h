#pragma once
#include "..\Scenegraph\GameObject.h"
#include "..\..\OverlordProject\Materials\SkyBoxMaterial.h"
#include "..\Components\ModelComponent.h"

class SkyBoxPrefab: public GameObject
{
public:
	SkyBoxPrefab(wstring assetFile);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:		

	wstring m_AssetFile;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxPrefab(const SkyBoxPrefab& yRef);									
	SkyBoxPrefab& operator=(const SkyBoxPrefab& yRef);
};
