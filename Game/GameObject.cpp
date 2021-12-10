#include "pch.h"
#include "GameObject.h"

using namespace hosTileSample;

GameObject::GameObject(std::shared_ptr<hosTile::hTSprite> sprite)
:	m_sprite(sprite)
{
	;
}

DirectX::XMFLOAT3 GameObject::GetPosition() const
{
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

	if (m_sprite != nullptr)
	{
		position = m_sprite->GetPosition();
	}

	return position;
}

std::shared_ptr<hosTile::hTSprite> GameObject::GetSprite() const
{
	return m_sprite;
}
