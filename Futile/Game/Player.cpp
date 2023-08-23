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
:	m_playerState(PS_Idle)
{
	m_sprite = make_unique<hTAnimatedSprite>(
		Game::Get().GetRenderer()->GetDeviceResources(), "WarriorRightIdle.json");
}

void Player::Update(const DX::StepTimer& timer)
{
	float delta = (float)timer.GetElapsedSeconds();

	switch (m_playerState)
	{
	case PS_Idle:
		{
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_playerState = PS_Walking;
				m_walkingTarget = Game::Get().GetRenderer()->ScreenToWorldPosition(
					Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
				XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
				XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
				XMVECTOR direction = walkingTargetVector - positionVector;
				XMVECTOR directionNormalized = XMVector3Normalize(direction);
				float angle = XMConvertToDegrees(
					atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
				PlayAnimationForDirection(
					angle,
					"WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");
			}
		}
		break;

	case PS_Walking:
		{
			Mouse::ButtonStateTracker mouseTracker = Input::Get().GetMouseTracker();
			if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED ||
				mouseTracker.leftButton == Mouse::ButtonStateTracker::HELD)
			{
				m_walkingTarget = Game::Get().GetRenderer()->ScreenToWorldPosition(
					Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
			}

			// Move towards the walking target.
			XMVECTOR walkingTargetVector = XMLoadFloat3(&m_walkingTarget);
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			XMVECTOR direction = walkingTargetVector - positionVector;
			XMVECTOR directionNormalized = XMVector3Normalize(direction);
			float angle = XMConvertToDegrees(
				atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
			PlayAnimationForDirection(
				angle,
				"WarriorLeftWalk.json", "WarriorDownWalk.json", "WarriorRightWalk.json", "WarriorUpWalk.json");

			XMFLOAT3 position = GetPosition();
			position.x += XMVectorGetX(directionNormalized) * MovementSpeed * delta;
			position.y += XMVectorGetY(directionNormalized) * MovementSpeed * delta;
			SetPosition(position);

			// When we're close enough, stop.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < MovementSpeed * delta)
			{
				m_playerState = PS_Idle;
				PlayAnimationForDirection(
					angle,
					"WarriorLeftIdle.json", "WarriorDownIdle.json", "WarriorRightIdle.json", "WarriorUpIdle.json");
			}
		}
		break;
	}

	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	animatedSprite->Update(delta);
}

void Player::Render(hosTile::hTRenderer& renderer)
{
	m_sprite->Render(renderer);
}

// Play a particular animation based on an atan2 angle in the range 0.0f to 360.0f.
void Player::PlayAnimationForDirection(
	float angle,
	string leftAnimation, string downAnimation, string rightAnimation, string upAnimation)
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	if (angle > 315.0f || angle <= 45.0f)
	{
		animatedSprite->PlayAnimation(
			Game::Get().GetRenderer()->GetDeviceResources(), leftAnimation);
	}
	else if (angle > 45.0f && angle <= 135.0f)
	{
		animatedSprite->PlayAnimation(
			Game::Get().GetRenderer()->GetDeviceResources(), downAnimation);
	}
	else if (angle > 135.0f && angle <= 225.0f)
	{
		animatedSprite->PlayAnimation(
			Game::Get().GetRenderer()->GetDeviceResources(), rightAnimation);
	}
	else
	{
		animatedSprite->PlayAnimation(
			Game::Get().GetRenderer()->GetDeviceResources(), upAnimation);
	}
}
