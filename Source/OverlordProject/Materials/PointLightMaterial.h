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
#include "Graphics\Material.h"

class TextureData;

struct PointLight
{
	PointLight()
	{
		ZeroMemory(this, sizeof(PointLight));
	}
	XMFLOAT3 dir;
	float pad1;
	///////////////**************new**************////////////////////
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 att;
	float pad2;
	///////////////**************new**************////////////////////
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};



class PointLightMaterial : public Material
{
public:
	PointLightMaterial();
	~PointLightMaterial();

	void SetDiffuseTexture(const wstring& assetFile);

	void SetVecEye(XMFLOAT4 vecEye);
	void SetLightColor(XMFLOAT4 lightColor);
	void SetLightPos(XMFLOAT3 pos, int index);
	void SetLightRange(XMFLOAT2 lightRange);

	void SetPointLight(PointLight pointLight);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

	TextureData* m_pDiffuseTexture;

	XMFLOAT3 m_PointLightPosition;
	/*	
	XMFLOAT4 vecEye2;
	XMFLOAT3 vecLightPos2;
	XMFLOAT2 lightRange2;
	XMFLOAT4 lightColor2;*/

	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	PointLight light[2];

	ID3DX11EffectVariable* mfxPointLight;
	ID3DX11EffectVariable* mfxPointLight2;
	//ID3DX11EffectVectorVariable* m_pLightRange;
	//ID3DX11EffectVectorVariable* m_pVecLightPos;
	//ID3DX11EffectVectorVariable* m_pLightColor;
	//ID3DX11EffectVectorVariable* m_pVecEye;;

	//static ID3DX11EffectShaderResourceVariable* lightPos;
	//	static ID3DX11EffectShaderResourceVariable* lightRange;
	//		static ID3DX11EffectShaderResourceVariable* lightAtt;
	//			static ID3DX11EffectShaderResourceVariable* lightAmbient;
	//				static ID3DX11EffectShaderResourceVariable* lightDiffuse;

	//ID3DX11EffectVectorVariable* m_pLightPosition;
	//ID3DX11EffectVectorVariable* m_pLightRange;
	//ID3DX11EffectVectorVariable* m_pLightAttenuation;
	//ID3DX11EffectVectorVariable* m_pLightAmbient;
	//ID3DX11EffectVectorVariable* m_pLightDiffuse;



private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PointLightMaterial(const PointLightMaterial &obj);
	PointLightMaterial& operator=(const PointLightMaterial& obj);
};

