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
