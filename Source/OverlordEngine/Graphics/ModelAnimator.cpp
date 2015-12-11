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

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	// Set m_ClipSet to false
	m_ClipSet = false;
	
	// Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		m_CurrentClip = m_pMeshFilter->m_AnimationClips[clipNumber];
		// Call SetAnimation(AnimationClip clip)
		SetAnimation(m_CurrentClip);
	}
	else // If not, call Reset
	{
		Reset();
		// Log a warning with an appropriate message
		Logger::LogError(L"ModelAnimator::SetAnimation(UINT clipNumber) > clipNumber is greater than m_AnimationClips vector size!");
		// return;
	}	
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	// Set m_ClipSet to false
	m_ClipSet = false;
	// Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (int index = 0 ; index < m_pMeshFilter->m_AnimationClips.size() ; index++)
	{
		if (m_pMeshFilter->m_AnimationClips[index].Name == clipName) // If found,
		{
			// Call SetAnimation(Animation clip) with the found clip
			SetAnimation(m_pMeshFilter->m_AnimationClips[index]);
		}
		else
		{
			// Call Reset
			Reset();
			// Log a warning with an appropriate message
			Logger::LogError(L"ModelAnimator::SetAnimation(wstring clipName) > AnimationClip with given name not found!");
		}
	}	
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	// Set m_ClipSet to true
	m_ClipSet = true;
	
	// Set m_CurrentClip
	m_CurrentClip = clip;
	
	// Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	if (pause == true)
	{
		m_IsPlaying = false; // If pause is true, set m_IsPlaying to false		
	}

	m_TickCount = 0; // Set m_TickCount to zero
	m_AnimationSpeed = 1.0f; // Set m_AnimationSpeed to 1.0f

	// If m_ClipSet is true
	if (m_ClipSet == true)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)			
		// Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(m_CurrentClip.Keys[0].BoneTransforms.begin(), m_CurrentClip.Keys[0].BoneTransforms.end());
	}
	// Else
	else
	{
		// Create an IdentityMatrix 
		XMMATRIX tempMat = XMMatrixIdentity();
		XMFLOAT4X4 matIdentity;
		XMStoreFloat4x4(&matIdentity, tempMat);
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, matIdentity);
	}
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	// We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		// 1. 
		// Calculate the passedTicks (see the lab document)
		auto passedTicks = gameContext.pGameTime->GetElapsed() * gameContext.pGameTime->GetFPS() * m_AnimationSpeed;
		// Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		fmod(passedTicks, m_CurrentClip.Duration);

		// 2. 
		// IF m_Reversed is true
		if (m_Reversed == true)
		{
			// Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			// If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0) m_TickCount += m_CurrentClip.Duration; 
		}
		// ELSE
		else
		{
			// Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			// if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.Duration) m_TickCount -= m_CurrentClip.Duration;
		}  
		
		// 3.
		// Find the enclosing keys
		AnimationKey keyA, keyB;
		// Iterate all the keys of the clip and find the following keys:
		for (auto key : m_CurrentClip.Keys)
		{
			// keyA > Closest Key with Tick before/smaller than m_TickCount
			if (key.Tick <= m_TickCount) keyA = key;
			// keyB > Closest Key with Tick after/bigger than m_TickCount
			else
			{
				keyB = key;
				break; 
			}
		}

		// 4.
		// Interpolate between keys
		float ticksBetweenKeys = (keyB.Tick - keyA.Tick);

		// Figure out the BlendFactor (See lab document)
		float blendFactor = (m_TickCount - keyA.Tick) / ticksBetweenKeys;

		// Clear the m_Transforms vector
		m_Transforms.clear();

		// FOR every boneTransform in a key (So for every bone)
		for (UINT index = 0; index < m_CurrentClip.Keys[index].BoneTransforms.size(); ++index)
		{
			// Retrieve the transform from keyA (transformA)
			auto transformA = XMLoadFloat4x4(&keyA.BoneTransforms[index]);
			// Retrieve the transform from keyB (transformB)
			auto transformB = XMLoadFloat4x4(&keyB.BoneTransforms[index]);

			// Decompose both transforms
			XMVECTOR posA, posB, scaleA, scaleB, rotA, rotB;
			XMMatrixDecompose(&scaleA, &rotA, &posA, transformA);
			XMMatrixDecompose(&scaleB, &rotB, &posB, transformB);

			// Lerp between all the transformations (Position, Scale, Rotation)
			XMVECTOR lerpScale = XMVectorLerp(scaleA, scaleB, blendFactor);
			XMVECTOR lerpRot = XMQuaternionSlerp(rotA, rotB, blendFactor);
			XMVECTOR lerpTrans = XMVectorLerp(posA, posB, blendFactor);

			// Compose a transformation matrix with the lerp-results
			XMMATRIX composedMat, scaleMat, rotMat, transMat;
			scaleMat = XMMatrixScalingFromVector(lerpScale);
			rotMat = XMMatrixRotationQuaternion(lerpRot);
			transMat = XMMatrixTranslationFromVector(lerpTrans);
			composedMat = scaleMat * rotMat * transMat;

			// Add the resulting matrix to the m_Transforms vector
			XMFLOAT4X4 composedFloat4x4;
			XMStoreFloat4x4(&composedFloat4x4, composedMat);
			m_Transforms.push_back(composedFloat4x4);
		}
	}
}