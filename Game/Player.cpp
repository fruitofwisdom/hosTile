#include "pch.h"
#include "Player.h"

#include "Keyboard.h"

using namespace hosTileSample;

const float Player::MovementSpeed = 64.0f;		// pixels-per-second

Player::Player(std::shared_ptr<hosTile::hosTileSprite> sprite)
:	GameObject(sprite)
{
	;
}

void Player::Update(const DX::StepTimer& timer)
{
	float delta = float(timer.GetElapsedSeconds());

	DirectX::XMFLOAT3 position = m_sprite->GetPosition();
	auto kb = DirectX::Keyboard::Get().GetState();
	if (kb.Up || kb.W)
	{
		m_sprite->SetCurrentSubSprite(1);
		m_sprite->SetXFlip(false);
		position.y += MovementSpeed * delta;
	}
	else if (kb.Down || kb.S)
	{
		m_sprite->SetCurrentSubSprite(0);
		m_sprite->SetXFlip(false);
		position.y -= MovementSpeed * delta;
	}
	else if (kb.Left || kb.A)
	{
		m_sprite->SetCurrentSubSprite(2);
		m_sprite->SetXFlip(false);
		position.x -= MovementSpeed * delta;
	}
	else if (kb.Right || kb.D)
	{
		m_sprite->SetCurrentSubSprite(2);
		m_sprite->SetXFlip(true);
		position.x += MovementSpeed * delta;
	}
	m_sprite->SetPosition(position);
}
