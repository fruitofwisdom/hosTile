#include "pch.h"
#include "GameObject.h"

using namespace DirectX;
using namespace Futile;
using namespace hosTile;

GameObject::GameObject(XMFLOAT3 position)
:	m_position(position)
{
	;
}

XMFLOAT3 GameObject::GetPosition() const
{
	return m_position;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	m_position = position;
}

hTRegion GameObject::GetCollision() const
{
	hTRegion emptyRegion;
	return emptyRegion;
}

hTRegion GameObject::GetHitBox() const
{
	hTRegion emptyRegion;
	return emptyRegion;
}

hTRegion GameObject::GetHurtBox() const
{
	hTRegion emptyRegion;
	return emptyRegion;
}

void GameObject::ReceiveHit(const GameObject* attacker)
{
	// do nothing here
}
