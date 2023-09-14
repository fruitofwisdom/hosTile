#include "pch.h"
#include "GameObject.h"

using namespace Futile;
using namespace hosTile;

GameObject::GameObject()
{
	;
}

GameObject::GameObject(std::unique_ptr<hosTile::hTSprite> sprite)
:	m_sprite(std::move(sprite))
{
	;
}

void GameObject::Render(hosTile::hTRenderer& renderer)
{
	if (m_sprite != nullptr)
	{
		m_sprite->Render(renderer);
	}
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

void GameObject::SetPosition(DirectX::XMFLOAT3 position)
{
	if (m_sprite != nullptr)
	{
		m_sprite->SetPosition(position);
	}
}

hTSprite* GameObject::GetSprite() const
{
	hTSprite* sprite = nullptr;

	if (m_sprite != nullptr)
	{
		sprite = m_sprite.get();
	}

	return sprite;
}

hTRegion GameObject::GetCollision() const
{
	hTRegion collision;
	return collision;
}

hTRegion GameObject::GetHitBox() const
{
	hTRegion hitBox;
	return hitBox;
}

hTRegion GameObject::GetHurtBox() const
{
	hTRegion hurtBox;
	return hurtBox;
}
