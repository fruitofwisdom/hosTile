#include "pch.h"
#include "Enemy.h"

#include "Game.h"
#include "..\hosTile\hTAnimatedSprite.h"

using namespace Futile;
using namespace hosTile;

Enemy::Enemy(DirectX::XMFLOAT3 position, float scale)
{
	m_sprite = std::make_unique<hTAnimatedSprite>(Game::Get().GetRenderer().GetDeviceResources(), "GoblinDownIdle.json");
	SetPosition(position);
	m_sprite->SetScale(scale);
}

void Enemy::Update(const DX::StepTimer& timer)
{
	hTAnimatedSprite* animatedSprite = static_cast<hTAnimatedSprite*>(m_sprite.get());
	float elapsedSeconds = (float)timer.GetElapsedSeconds();
	animatedSprite->Update(elapsedSeconds);
}
