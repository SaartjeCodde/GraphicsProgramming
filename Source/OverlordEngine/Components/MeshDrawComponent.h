//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.81
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include "BaseComponent.h"
#include "../Helpers/VertexHelper.h"

class MeshDrawComponent : public BaseComponent
{
public:
	MeshDrawComponent(UINT triangleCapacity = 50);
	virtual ~MeshDrawComponent(void);

	void AddTriangle(TrianglePosNormCol triangle, bool updateBuffer = false);
	void AddTriangle(VertexPosNormCol vertex1, VertexPosNormCol vertex2, VertexPosNormCol vertex3, bool updateBuffer = false);
	void AddQuad(VertexPosNormCol vertex1, VertexPosNormCol vertex2, VertexPosNormCol vertex3, VertexPosNormCol vertex4, bool updateBuffer = false);
	void AddQuad(QuadPosNormCol quad, bool updateBuffer = false);
	void RemoveTriangles();

	void UpdateBuffer();

	UINT GetTriangleCapacity() const { return m_TriangleCapacity;}
	void SetTriangleCapacity(UINT capacity) { m_TriangleCapacity = capacity; } //TODO:FIX //if(m_IsInitialized)InitializeBuffer(); }

protected:
	
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);
	virtual void Initialize(const GameContext& gameContext);

private:

	
	void LoadEffect(const GameContext& gameContext);
	void InitializeBuffer(const GameContext& gameContext);

	vector<TrianglePosNormCol> m_vecTriangles;
	ID3D11Buffer *m_pVertexBuffer;
	UINT m_TriangleCapacity;

	static ID3DX11Effect* m_pEffect;
	static ID3DX11EffectTechnique* m_pTechnique;
	static ID3D11InputLayout* m_pInputLayout;
	static ID3DX11EffectMatrixVariable *m_pWorldVar,*m_pWvpVar;
	
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshDrawComponent(const MeshDrawComponent& yRef);									
	MeshDrawComponent& operator=(const MeshDrawComponent& yRef);

};

