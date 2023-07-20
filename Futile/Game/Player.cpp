#include "pch.h"
#include "Player.h"

#include "Game.h"
#include "Input.h"

using namespace DirectX;
using namespace Futile;

const float Player::MovementSpeed = 128.0f;		// pixels-per-second
const float Player::NearTarget = 0.5f;		// distance in world space

Player::Player(std::unique_ptr<hosTile::hTTileSprite> sprite)
:	GameObject(std::move(sprite)),
	m_playerState(PS_Idle)
{
	;
}

void Player::Update(const DX::StepTimer& timer)
{
	float delta = float(timer.GetElapsedSeconds());

	// An example of keyboard movement
	/*
	XMFLOAT3 position = GetPosition();
	Keyboard::State keyboardState = Input::Get().GetKeyboardState();
	if (keyboardState.Up || keyboardState.W)
	{
		m_sprite->SetXFlip(false);
		position.y += MovementSpeed * delta;
	}
	else if (keyboardState.Down || keyboardState.S)
	{
		m_sprite->SetXFlip(false);
		position.y -= MovementSpeed * delta;
	}
	else if (keyboardState.Left || keyboardState.A)
	{
		m_sprite->SetXFlip(false);
		position.x -= MovementSpeed * delta;
	}
	else if (keyboardState.Right || keyboardState.D)
	{
		m_sprite->SetXFlip(true);
		position.x += MovementSpeed * delta;
	}
	SetPosition(position);
	*/

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
			XMFLOAT3 position = GetPosition();
			position.x += XMVectorGetX(directionNormalized) * MovementSpeed * delta;
			position.y += XMVectorGetY(directionNormalized) * MovementSpeed * delta;
			SetPosition(position);

			// When we're close enough, stop.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < NearTarget)
			{
				m_playerState = PS_Idle;
			}
		}
		break;
	}
}

void Player::Render(hosTile::hTRenderer& renderer)
{
	m_sprite->Render(renderer);
}
