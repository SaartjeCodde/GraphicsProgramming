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
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base/stdafx.h"
#include "Character.h"
#include "Components/Components.h"
#include "Prefabs/Prefabs.h"
#include "Scenegraph/GameScene.h"
#include "Physx/PhysxManager.h"
#include "Physx/PhysxProxy.h"
#include "Diagnostics/Logger.h"

Character::Character(float radius, float height, float moveSpeed) : 
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0), 
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f), 
	m_TerminalVelocity(20),
	m_Gravity(9.81f), 
	m_RunAccelerationTime(0.3f), 
	m_JumpAccelerationTime(0.8f), 
	m_RunAcceleration(m_MaxRunVelocity/m_RunAccelerationTime), 
	m_JumpAcceleration(m_Gravity/m_JumpAccelerationTime), 
	m_RunVelocity(0), 
	m_JumpVelocity(0),
	m_Velocity(0,0,0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto basicMaterial = physX->createMaterial(0,0,0);

	// Create controller
	m_pController = new ControllerComponent(basicMaterial);
	AddComponent(m_pController);

	// Add a fixed camera as child
	auto fixedCamera = new FixedCamera();
	AddChild(fixedCamera);

	// Register all Input Actions
	auto inputAction1 = InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'Z'); 
	gameContext.pInput->AddInputAction(inputAction1); 
	auto inputAction2 = InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S'); 
	gameContext.pInput->AddInputAction(inputAction2); 
	auto inputAction3 = InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'Q'); 
	gameContext.pInput->AddInputAction(inputAction3); 
	auto inputAction4 = InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D'); 
	gameContext.pInput->AddInputAction(inputAction4); 
	auto inputAction5 = InputAction(CharacterMovement::JUMP, InputTriggerState::Pressed, VK_SPACE); 
	gameContext.pInput->AddInputAction(inputAction5); 
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	m_pCamera = GetChild<FixedCamera>()->GetComponent<CameraComponent>();
	m_pCamera->SetActive();
}

void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Update the character (Camera rotation, Character Movement, Character Gravity)
	
	if(m_pCamera->IsActive())
	{
		// Handle input by using CharacterMovement enumerations
		XMFLOAT2 move = XMFLOAT2(0,0);
		move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	
		move.y -= gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = gameContext.pInput->GetThumbstickPosition().y;
		
		move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT) ? 1.0f : 0.0f;
		move.x -= gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = gameContext.pInput->GetThumbstickPosition().x;

		XMFLOAT2 look = XMFLOAT2(0,0);
		if (gameContext.pInput->IsMouseButtonDown(VK_LBUTTON))
		{
			auto mouseMove = gameContext.pInput->GetMouseMovement();
			look.x = static_cast<float>(mouseMove.x); 
			look.y = static_cast<float>(mouseMove.y);
		}

		if (look.x == 0 && look.y == 0)
		{
			look = gameContext.pInput->GetThumbstickPosition(false);
		}

		// Calculate transformations
		// 1. Determine the direction in which we are moving
		auto transform = GetTransform();
		
		XMVECTOR forward = XMLoadFloat3(&GetTransform()->GetForward());
		XMVECTOR right = XMLoadFloat3(&GetTransform()->GetRight());
	
		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		
		// Camera rotation
		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		
		XMVECTOR currDirection = (forward * move.y) + (right * move.x);
		currDirection = XMVector3Normalize(currDirection);

		// 2. 
		if (move.x != 0 || move.y != 0) // If x or y movement is different from 0, calcultate run velocity.
		{
			// By multiplying run acceleration by elapsed game time and adding it to run velocity.
			m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
			if (m_RunVelocity > m_MaxRunVelocity) m_RunVelocity = m_MaxRunVelocity;
			
			float temp = m_Velocity.y; // Store y component of m_Velocity
				
			// m_Velocity will be the multiplication of our direction and our run velocity;
			XMStoreFloat3(&m_Velocity, currDirection * m_RunVelocity);
			m_Velocity.y = temp; 			
		}
		// 3. If we did not have movement, set the x and z component of m_Velocity to 0
		else
		{
			m_Velocity.x = 0;			
			m_Velocity.z = 0;
		}

		// 4. If controller is not colliding at his bottom, we want to apply gravity.
		// Check this using the collision flags on the controller
		if (!m_pController->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN)) // If we do not have collision,
		{			
			m_JumpVelocity -= m_JumpAcceleration * gameContext.pGameTime->GetElapsed();
			if (m_JumpVelocity < -m_TerminalVelocity) m_JumpVelocity -= m_TerminalVelocity; // decrement m_JumpVelocity with our jump acceleration multiplied by the elapsed time,
			// & make sure the velocity does not excede the terminal velocity
		}
		// 5. If our jump action is triggered, set jump velocity to 0 and m_Velocity.y to 200
		else if (gameContext.pInput->IsActionTriggered(CharacterMovement::JUMP)) 
		{
			m_JumpVelocity = 0;	
			m_Velocity.y = 200;
		}	
		// 6. Else we will set m_Velocity.y to 0
		else
		{
			m_Velocity.y = 0;
		}
		
		// 7. Finally, add m_JumpVelocity to m_Velocity.y
		m_Velocity.y += m_JumpVelocity;
		auto vecVelocity = XMLoadFloat3(&m_Velocity);
		XMFLOAT3 moveAmount;

		// 8. The last thing we need to do is move our controller by our velocity multiplied by our elapsed time.
		XMStoreFloat3(&moveAmount, (vecVelocity * gameContext.pGameTime->GetElapsed()));
		m_pController->Move(moveAmount);		
	}	
}

float Character::GetTotalYaw()
{
	return m_TotalYaw;
}
