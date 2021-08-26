#include "pch.h"
#include "Player.h"

#include "Keyboard.h"

using namespace hosTileSample;

Player::Player(std::shared_ptr<hosTile::hosTileSprite> sprite, DirectX::XMFLOAT3 position)
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
		position.y += 1.0f;
	}
	else if (kb.Down || kb.S)
	{
		position.y -= 1.0f;
	}
	if (kb.Left || kb.A)
	{
		position.x -= 1.0f;
	}
	else if (kb.Right || kb.D)
	{
		position.x += 1.0f;
	}
	m_sprite->SetPosition(position);
}
