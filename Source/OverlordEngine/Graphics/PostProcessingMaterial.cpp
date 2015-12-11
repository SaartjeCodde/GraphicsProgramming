//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.113
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "../Base/OverlordGame.h"
#include "../Content/ContentManager.h"
#include "../Helpers/VertexHelper.h"

PostProcessingMaterial::PostProcessingMaterial(wstring effectFile, unsigned int renderIndex, const wstring& technique)
	: m_IsInitialized(false),
	m_pEffect(nullptr),
	m_effectFile(effectFile),
	m_pInputLayoutSize(0),
	m_InputLayoutID(0),
	m_RenderIndex(renderIndex),
	m_pRenderTarget(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_NumVertices(0),
	m_NumIndices(0),
	m_VertexBufferStride(0),
	m_pTechnique(nullptr),
	m_TechniqueName(technique)
{
}


PostProcessingMaterial::~PostProcessingMaterial()
{
	SafeRelease(m_pInputLayout);
	m_pInputLayoutDescriptions.clear();
	SafeDelete(m_pRenderTarget);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if(!m_IsInitialized)
	{
		// Load Effect
		LoadEffect(gameContext, m_effectFile);
		
		// Create Vertex Buffer
		CreateVertexBuffer(gameContext);
		
		// Create Index Buffer
		CreateIndexBuffer(gameContext);
		
		// Create RenderTarget
		m_pRenderTarget = new RenderTarget(gameContext.pDevice);
		RENDERTARGET_DESC targetDesc;
		targetDesc.Height = GameSettings::WindowSettings().Height;
		targetDesc.Width = GameSettings::WindowSettings().Width;
		m_pRenderTarget->Create(targetDesc);

		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const wstring& effectFile)
{
	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	if(m_pEffect == nullptr)
	{
		Logger::LogWarning(L"PostProcessingMaterial::LoadEffect - effect not loaded!");
		return false;
	}

	//string techName = string(m_TechniqueName.begin(), m_TechniqueName.end());
	//m_pTechnique = m_pEffect->GetTechniqueByName(techName.c_str());
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	UINT size/* = m_pInputLayoutSize*/;

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout/**/, size /*, m_pInputLayoutDescriptions, m_pInputLayoutSize, m_InputLayoutID*/);
	LoadEffectVariables();

	return true;
}

void PostProcessingMaterial::SetRenderIndex(unsigned int index)
{
	m_RenderIndex = index;
}

unsigned int PostProcessingMaterial::GetRenderIndex()
{
	return m_RenderIndex;
}

void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	// Clear the render target
	float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pRenderTarget->Clear(gameContext, color);
	
	// update the effect variables
	UpdateEffectVariables(previousRendertarget);
	
	// set input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	
	// set vertex buffer
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset);
	//gameContext.pDeviceContext->IASetVertexBuffers(0, 0, &m_pVertexBuffer, (UINT*) m_NumVertices, 0);
	
	// set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// draw
	//gameContext.pDeviceContext->Draw( (UINT) m_NumVertices, 0);
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for(UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
	
	// generate mips
	gameContext.pDeviceContext->GenerateMips(/*previousRendertarget*/m_pRenderTarget->GetShaderResourceView());

	// call PsSetShaderResources to reset the shader resources
	ID3D11ShaderResourceView *const pSRV[1] = {NULL};
    gameContext.pDeviceContext->PSSetShaderResources(0, 1, pSRV);
}

void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// create a vertex buffer for a full screen quad using VertexPosTex
	vector<VertexPosTex> vertices;
	vertices.push_back(VertexPosTex(XMFLOAT3(1, -1, 0), XMFLOAT2(1,1) ));
	vertices.push_back(VertexPosTex(XMFLOAT3(1, 1, 0), XMFLOAT2(1,0) ));
	vertices.push_back(VertexPosTex(XMFLOAT3(-1, -1, 0), XMFLOAT2(0,1) ));
	vertices.push_back(VertexPosTex(XMFLOAT3(-1, 1, 0), XMFLOAT2(0,0) ));

	m_NumVertices = vertices.size();
	m_VertexBufferStride = sizeof(VertexPosTex);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexPosTex) * m_NumVertices;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	//desc.StructureByteStride = m_VertexBufferStride;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	gameContext.pDevice->CreateBuffer(&desc, &initData, &m_pVertexBuffer); 	
}

void PostProcessingMaterial::CreateIndexBuffer(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Create the index buffer for a full screen quad
	vector<DWORD> indices;
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	m_NumIndices = indices.size();

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(DWORD) * m_NumIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices.data();

	gameContext.pDevice->CreateBuffer(&desc, &initData, &m_pIndexBuffer); 
}

RenderTarget*  PostProcessingMaterial::GetRenderTarget()
{
	return m_pRenderTarget;
}

