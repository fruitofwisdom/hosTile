#include "pch.h"
#include "WorldObject.h"

#include "Game.h"
#include "..\hosTile\hTAnimatedSprite.h"
#include <string>

using namespace Futile;
using namespace hosTile;
using namespace std;

WorldObject::WorldObject(string animation, DirectX::XMFLOAT3 position, float scale)
:	GameObject(position),
	m_facingAngle(90.0f)
{
	m_sprite = make_unique<hTAnimatedSprite>(Game::Get().GetRenderer().GetDeviceResources(), animation, GetPosition());
	m_sprite->SetScale(scale);
}

void WorldObject::Update(const DX::StepTimer& timer)
{
	m_sprite->SetPosition(GetPosition());
	m_sprite->Update((float)timer.GetElapsedSeconds());
}

void WorldObject::Render(hTRenderer& renderer)
{
	m_sprite->Render(renderer);
}

hTRegion WorldObject::GetCollision() const
{
	return m_sprite->GetCollision();
}

hTRegion WorldObject::GetHitBox() const
{
	return m_sprite->GetHitBox();
}

hTRegion WorldObject::GetHurtBox() const
{
	return m_sprite->GetHurtBox();
}

// Move to a new position, considering collision.
void WorldObject::Move(DirectX::XMFLOAT3 position)
{
	// TODO: Based on our original GetPosition(), our new position, our GetCollision(), and the
	// level collision, figure out the correct position on the way.
	SetPosition(position);
}

// Play a particular animation based on the facing angle in the range 0.0f to 360.0f.
void WorldObject::PlayAnimationForDirection(string leftAnimation, string downAnimation, string rightAnimation, string upAnimation, bool looping)
{
	if (m_facingAngle > 315.0f || m_facingAngle <= 45.0f)
	{
		m_sprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), leftAnimation, looping);
	}
	else if (m_facingAngle > 45.0f && m_facingAngle <= 135.0f)
	{
		m_sprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), downAnimation, looping);
	}
	else if (m_facingAngle > 135.0f && m_facingAngle <= 225.0f)
	{
		m_sprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), rightAnimation, looping);
	}
	else
	{
		m_sprite->PlayAnimation(Game::Get().GetRenderer().GetDeviceResources(), upAnimation, looping);
	}
}