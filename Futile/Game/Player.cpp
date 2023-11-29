#include "pch.h"
#include "Player.h"

#include "Game.h"
#include "Input.h"
#include "Utilities.h"

using namespace DirectX;
using namespace Futile;
using namespace std;

const float Player::MovementSpeed = 64.0f;		// pixels-per-second

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

	case PS_Hurt:
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
			// If the left mouse button is pressed, stalk to walk.
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Walk;
				m_walkingTarget = Game::Get().GetRenderer().ScreenToWorldPosition(Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);

				// Move towards the walking target.
				XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
				XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
				XMVECTOR directionNormalized = XMVector3Normalize(walkingTargetVector - positionVector);
				m_facingAngle = Utilities::GetAngleTo(directionNormalized);
				PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");
			}
			// If the right mouse button is pressed, start to attack.
			else if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Attack;
				PlayAnimationForDirection(
					"WarriorLeftAttack01.json", "WarriorDownAttack01.json", "WarriorRightAttack01.json", "WarriorUpAttack01.json", false);
			}
		}
		break;

	case PS_Walk:
		{
			// If the right mouse button is pressed, stop and attack.
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_state = PS_Attack;
				PlayAnimationForDirection(
					"WarriorLeftAttack01.json", "WarriorDownAttack01.json", "WarriorRightAttack01.json", "WarriorUpAttack01.json", false);
			}
			else
			{
				float elapsedSeconds = (float)timer.GetElapsedSeconds();

				if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED ||
					mouseTracker.leftButton == Mouse::ButtonStateTracker::HELD)
				{
					m_walkingTarget = Game::Get().GetRenderer().ScreenToWorldPosition(Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
				}

				// Move towards our walking target.
				XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
				XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
				XMVECTOR directionNormalized = XMVector3Normalize(walkingTargetVector - positionVector);
				m_facingAngle = Utilities::GetAngleTo(directionNormalized);
				PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");

				XMFLOAT3 position = GetPosition();
				position.x += XMVectorGetX(directionNormalized) * MovementSpeed * elapsedSeconds;
				position.y += XMVectorGetY(directionNormalized) * MovementSpeed * elapsedSeconds;
				Move(position);

				// Stop when we're close enough.
				float distanceToTarget = XMVectorGetX(XMVector3Length(walkingTargetVector - positionVector));
				if (distanceToTarget < MovementSpeed * elapsedSeconds)
				{
					m_state = PS_Idle;
					PlayAnimationForDirection("WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
				}
			}
		}
		break;
	}

	WorldObject::Update(timer);
}

void Player::ReceiveHit(const GameObject* attacker)
{
	m_state = PS_Hurt;
	PlayAnimationForDirection(
		"WarriorLeftHurt.json", "WarriorDownHurt.json", "WarriorRightHurt.json", "WarriorUpHurt.json", false);
}
