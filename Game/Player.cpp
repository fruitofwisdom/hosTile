#include "pch.h"
#include "Player.h"

#include "Keyboard.h"

using namespace Futile;

const float Player::MovementSpeed = 128.0f;		// pixels-per-second

Player::Player(std::unique_ptr<hosTile::hTTileSprite> sprite)
:	GameObject(std::move(sprite))
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
		m_sprite->SetXFlip(false);
		position.y += MovementSpeed * delta;
	}
	else if (kb.Down || kb.S)
	{
		m_sprite->SetXFlip(false);
		position.y -= MovementSpeed * delta;
	}
	else if (kb.Left || kb.A)
	{
		m_sprite->SetXFlip(false);
		position.x -= MovementSpeed * delta;
	}
	else if (kb.Right || kb.D)
	{
		m_sprite->SetXFlip(true);
		position.x += MovementSpeed * delta;
	}
	SetPosition(position);
}

void Player::Render(hosTile::hTRenderer& renderer)
{
	m_sprite->Render(renderer);
}
