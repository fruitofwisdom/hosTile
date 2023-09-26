#include "pch.h"
#include "Player.h"

#include "Game.h"
#include "..\hosTile\hTAnimatedSprite.h"
#include "Input.h"

using namespace DirectX;
using namespace Futile;
using namespace std;

const float Player::MovementSpeed = 256.0f;		// pixels-per-second

Player::Player(XMFLOAT3 position, float scale)
:	WorldObject("WarriorDownIdle.json", position, scale),
	m_state(PS_Idle)
{
	;
}

void Player::Update(const DX::StepTimer& timer)
{
	switch (m_state)
	{
	case PS_Attack:
		{
			if (m_sprite->AnimationDone())
			{
				m_state = PS_Idle;
				PlayAnimationForDirection("WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
			}
		}
		break;

	case PS_Idle:
		{
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Walk;
				m_walkingTarget = Game::Get().GetRenderer().ScreenToWorldPosition(Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
				XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
				XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
				XMVECTOR direction = walkingTargetVector - positionVector;
				XMVECTOR directionNormalized = XMVector3Normalize(direction);
				m_facingAngle = XMConvertToDegrees((float)atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
				PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");
			}

			// If the right mouse button is pressed, start to attack.
			if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Attack;
				PlayAnimationForDirection(
					"WarriorLeftAttack01.json", "WarriorDownAttack01.json", "WarriorRightAttack01.json", "WarriorUpAttack01.json", false);
			}
		}
		break;

	case PS_Walk:
		{
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED ||
				mouseTracker.leftButton == Mouse::ButtonStateTracker::HELD)
			{
				m_walkingTarget = Game::Get().GetRenderer().ScreenToWorldPosition(Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
			}

			// Move towards the walking target.
			XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			XMVECTOR direction = walkingTargetVector - positionVector;
			XMVECTOR directionNormalized = XMVector3Normalize(direction);
			m_facingAngle = XMConvertToDegrees((float)atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
			PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");

			XMFLOAT3 position = GetPosition();
			float elapsedSeconds = (float)timer.GetElapsedSeconds();
			position.x += XMVectorGetX(directionNormalized) * MovementSpeed * elapsedSeconds;
			position.y += XMVectorGetY(directionNormalized) * MovementSpeed * elapsedSeconds;
			Move(position);

			// When we're close enough, stop.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < MovementSpeed * elapsedSeconds)
			{
				m_state = PS_Idle;
				PlayAnimationForDirection("WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
			}

			// If the right mouse button is pressed, stop and attack.
			if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Attack;
				PlayAnimationForDirection(
					"WarriorLeftAttack01.json", "WarriorDownAttack01.json", "WarriorRightAttack01.json", "WarriorUpAttack01.json", false);
			}
		}
		break;
	}

	WorldObject::Update(timer);
}
