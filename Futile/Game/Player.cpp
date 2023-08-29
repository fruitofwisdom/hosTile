#include "pch.h"
#include "Player.h"

#include "Game.h"
#include "..\hosTile\hTAnimatedSprite.h"
#include "Input.h"
#include <iomanip>
#include <math.h>
#include <sstream>

using namespace DirectX;
using namespace Futile;
using namespace hosTile;
using namespace std;

const float Player::MovementSpeed = 128.0f;		// pixels-per-second

Player::Player()
:	m_playerState(PS_Idle),
	m_facingAngle(90.0f)
{
	m_sprite = make_unique<hTAnimatedSprite>(Game::Get().GetRenderer().GetDeviceResources(), "WarriorDownIdle.json");
}

void Player::Update(const DX::StepTimer& timer)
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	float delta = (float)timer.GetElapsedSeconds();

	switch (m_playerState)
	{
	case PS_Attack:
		{
			if (animatedSprite->AnimationDone())
			{
				m_playerState = PS_Idle;
				PlayAnimationForDirection("WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
			}
		}
		break;

	case PS_Idle:
		{
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_playerState = PS_Walk;
				m_walkingTarget = Game::Get().GetRenderer().ScreenToWorldPosition(Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
				XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
				XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
				XMVECTOR direction = walkingTargetVector - positionVector;
				XMVECTOR directionNormalized = XMVector3Normalize(direction);
				m_facingAngle = XMConvertToDegrees(atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
				PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");
			}

			// If the right mouse button is pressed, start to attack.
			if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_playerState = PS_Attack;
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
			m_facingAngle = XMConvertToDegrees(atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
			PlayAnimationForDirection("WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");

			XMFLOAT3 position = GetPosition();
			position.x += XMVectorGetX(directionNormalized) * MovementSpeed * delta;
			position.y += XMVectorGetY(directionNormalized) * MovementSpeed * delta;
			SetPosition(position);

			// When we're close enough, stop.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < MovementSpeed * delta)
			{
				m_playerState = PS_Idle;
				PlayAnimationForDirection("WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
			}

			// If the right mouse button is pressed, stop and attack.
			if (mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_playerState = PS_Attack;
				PlayAnimationForDirection(
					"WarriorLeftAttack01.json", "WarriorDownAttack01.json", "WarriorRightAttack01.json", "WarriorUpAttack01.json", false);
			}
		}
		break;
	}

	animatedSprite->Update(delta);
}

void Player::Render(hosTile::hTRenderer& renderer)
{
	m_sprite->Render(renderer);
}

hTRegion Player::GetCollision() const
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	return animatedSprite->GetCollision();
}

hTRegion Player::GetHitBox() const
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	return animatedSprite->GetHitBox();
}

hTRegion Player::GetHurtBox() const
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	return animatedSprite->GetHurtBox();
}

// Play a particular animation based on the player's angle in the range 0.0f to 360.0f.
void Player::PlayAnimationForDirection(string leftAnimation, string downAnimation, string rightAnimation, string upAnimation, bool looping)
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	if (m_facingAngle > 315.0f || m_facingAngle <= 45.0f)
	{
		animatedSprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), leftAnimation, looping);
	}
	else if (m_facingAngle > 45.0f && m_facingAngle <= 135.0f)
	{
		animatedSprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), downAnimation, looping);
	}
	else if (m_facingAngle > 135.0f && m_facingAngle <= 225.0f)
	{
		animatedSprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), rightAnimation, looping);
	}
	else
	{
		animatedSprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), upAnimation, looping);
	}
}
