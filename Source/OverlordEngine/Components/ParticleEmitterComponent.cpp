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

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount):
	m_pEffect(nullptr),
	m_pVertexBuffer(nullptr),
	m_pInputLayout(nullptr),
	m_pInputLayoutSize(0),
	m_ParticleCount(particleCount),
	m_ActiveParticles(0),
	m_LastParticleInit(0.0f),
	m_Settings(ParticleEmitterSettings()),
	m_AssetFile(assetFile),
	m_pParticleTexture(nullptr)
{
	for (int index = 0; index < m_ParticleCount; ++index)
	{
		m_Particles.push_back( new Particle( m_Settings ) );
	}
}

ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	// Delete all the particles and clear the vector
	for (int index = 0; index < m_ParticleCount; ++index)
	{
		delete m_Particles[index];
		m_Particles.at(index) = nullptr;
	}
	m_Particles.clear();
	m_pInputLayout->Release();
	m_pVertexBuffer->Release();
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/ParticleRenderer.fx");

	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);

	m_pWvpVariable = m_pEffect->GetVariableByName("gWvp")->AsMatrix();

	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();

	m_pTextureVariable = m_pEffect->GetVariableByName("particleTexture")->AsShaderResource();
	
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer)
	{
		SafeRelease(m_pVertexBuffer);
	}

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;

	gameContext.pDevice->CreateBuffer(&buffDesc, NULL, &m_pVertexBuffer);	
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	// 1. Create a local variable, particleInterval (float), which contains the average particle emit threshold.
	float particleInterval = 0;
	particleInterval = (m_Settings.MinEnergy + m_Settings.MaxEnergy) / m_ParticleCount;
	
	// 2. Increase m_LastParticleInit by the elapsed gametime.
	m_LastParticleInit += gameContext.pGameTime->GetElapsed(); 

	// 3. Validate particles and add some of them to the vertexbuffer. 
	// Set m_ActiveParticles to zero
	m_ActiveParticles = 0;
	// Use DeviceContext::Map to map our vertexbuffer
	// --- BUFFER MAPPING CODE ---
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Create an empty pointer of type ParticleVertex, and cast & assign pData to it.
	ParticleVertex* pBuffer = (ParticleVertex*) mappedResource.pData;
	// Call the Update method of each Particle	
	for (UINT index = 0; index < m_Particles.size(); ++index)
	{
		m_Particles[index]->Update(gameContext);	

		if (m_Particles[index]->IsActive())
		{		
			pBuffer[m_ActiveParticles] = m_Particles[index]->GetVertexInfo();
			++m_ActiveParticles;
		}

		else if (m_LastParticleInit >= particleInterval)
		{
			m_Particles[index]->Init(GetTransform()->GetWorldPosition());
			pBuffer[m_ActiveParticles] = m_Particles[index]->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0;
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	// 1. Calculate the WVP matrix and use it to set the m_pWvpVariable. 
	auto world = XMLoadFloat4x4( &GetTransform()->GetWorld());
	//auto view = XMLoadFloat4x4(&gameContext.pCamera->GetView());
	auto viewProjection = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	auto wvp = world*viewProjection;
	m_pWvpVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	
	// 2. Retrieve the ViewInverse from the camera and use it to set the m_pViewInverseVariable
	XMMATRIX vieuwInverse = XMLoadFloat4x4( &gameContext.pCamera->GetViewInverse() );
	m_pViewInverseVariable->SetMatrix(reinterpret_cast<float*>(&vieuwInverse));

	// 3. Use m_pParticletexture to set the m_pTextureVariable
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	// 4. Set the InputLayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// 5. Set the PrimitiveTopology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	// 6. Set the VertexBuffer
	UINT offset = 0;
	UINT stride = sizeof(ParticleVertex);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	// 7. For each pass of our default technique:
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for(UINT pass = 0; pass < techDesc.Passes; ++pass)
	{
		// a. Apply the pass
		m_pDefaultTechnique->GetPassByIndex(pass)->Apply(0, gameContext.pDeviceContext);
		// b. Draw the vertices
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0); 
	}	
}
