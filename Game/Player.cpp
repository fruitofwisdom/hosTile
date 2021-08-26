#include "pch.h"
#include "Player.h"

#include "Keyboard.h"

using namespace hosTileSample;

Player::Player(std::shared_ptr<hosTile::hosTileSprite> sprite)
:	m_sprite(sprite)
{
	;
}

void Player::Update(const DX::StepTimer& timer)
{
	DirectX::XMFLOAT3 position = m_sprite->GetPosition();
	auto kb = DirectX::Keyboard::Get().GetState();
	if (kb.Up || kb.W)
	{
		m_sprite->SetCurrentSubSprite(1);
		m_sprite->SetXFlip(false);
		position.y += 1.0f;
	}
	else if (kb.Down || kb.S)
	{
		m_sprite->SetCurrentSubSprite(0);
		m_sprite->SetXFlip(false);
		position.y -= 1.0f;
	}
	if (kb.Left || kb.A)
	{
		m_sprite->SetCurrentSubSprite(2);
		m_sprite->SetXFlip(false);
		position.x -= 1.0f;
	}
	else if (kb.Right || kb.D)
	{
		m_sprite->SetCurrentSubSprite(2);
		m_sprite->SetXFlip(true);
		position.x += 1.0f;
	}
	m_sprite->SetPosition(position);
}
