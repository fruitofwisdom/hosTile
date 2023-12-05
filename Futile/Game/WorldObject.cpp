#include "pch.h"
#include "WorldObject.h"

#include <DirectXColors.h>
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

	// Debug options to render our various regions.
	//renderer.AddDebugQuad(GetCollision().AsQuad(DirectX::Colors::Blue));
	//renderer.AddDebugQuad(GetHitBox().AsQuad(DirectX::Colors::Red));
	//renderer.AddDebugQuad(GetHurtBox().AsQuad(DirectX::Colors::Yellow));
}

// Return the collision region in world space.
hTRegion WorldObject::GetCollision() const
{
	hTRegion collision = m_sprite->GetCollision();
	// Convert from the sprite's orientation (positive-y downwards) to our game's (positive-y up).
	collision.x = (int)GetPosition().x - (int)m_sprite->GetWidth() / 2.0f + collision.x;
	collision.y = (int)GetPosition().y + (int)m_sprite->GetHeight() / 2.0f - collision.y;
	return collision;
}

// Return the hit box in world space.
hTRegion WorldObject::GetHitBox() const
{
	hTRegion hitBox = m_sprite->GetHitBox();
	// Convert from the sprite's orientation (positive-y downwards) to our game's (positive-y up).
	hitBox.x = (int)GetPosition().x - (int)m_sprite->GetWidth() / 2.0f + hitBox.x;
	hitBox.y = (int)GetPosition().y + (int)m_sprite->GetHeight() / 2.0f - hitBox.y;
	return hitBox;
}

// Return the hurt box in world space.
hTRegion WorldObject::GetHurtBox() const
{
	hTRegion hurtBox = m_sprite->GetHurtBox();
	// Convert from the sprite's orientation (positive-y downwards) to our game's (positive-y up).
	hurtBox.x = (int)GetPosition().x - (int)m_sprite->GetWidth() / 2.0f + hurtBox.x;
	hurtBox.y = (int)GetPosition().y + (int)m_sprite->GetHeight() / 2.0f - hurtBox.y;
	return hurtBox;
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
