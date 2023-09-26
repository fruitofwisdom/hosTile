#include "pch.h"
#include "Enemy.h"

#include "Game.h"

using namespace DirectX;
using namespace Futile;

const float Enemy::AttackDistance = 100.0f;		// distance in world space
const float Enemy::AwareDistance = 500.0f;		// distance in world space
const float Enemy::MovementSpeed = 128.0f;		// pixels-per-second

Enemy::Enemy(XMFLOAT3 position, float scale)
:	WorldObject("GoblinDownIdle.json", position, scale),
	m_state(ES_Idle)
{
	;
}

void Enemy::Update(const DX::StepTimer& timer)
{
	switch (m_state)
	{
		case ES_Attack:
		{
			if (m_sprite->AnimationDone())
			{
				m_state = ES_Idle;
				PlayAnimationForDirection("GoblinLeftIdle.json", "GoblinDownIdle.json", "GoblinRightIdle.json", "GoblinUpIdle.json");
			}
		}
		break;

	case ES_Idle:
		{
			XMVECTOR playerVector = XMLoadFloat3(&Game::Get().GetPlayer()->GetPosition());
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			XMVECTOR direction = playerVector - positionVector;

			// If we're close enough to see the Player, start walking towards them.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < AwareDistance)
			{
				m_state = ES_Walk;
				XMVECTOR directionNormalized = XMVector3Normalize(direction);
				m_facingAngle = XMConvertToDegrees((float)atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
				PlayAnimationForDirection("GoblinLeftRun.json", "GoblinDownRun.json", "GoblinRightRun.json", "GoblinUpRun.json");

				// And if we're close enough to attack, start attacking them.
				if (XMVectorGetX(distance) < AttackDistance)
				{
					m_state = ES_Attack;
					PlayAnimationForDirection(
						"GoblinLeftAttack01.json", "GoblinDownAttack01.json", "GoblinRightAttack01.json", "GoblinUpAttack01.json", false);
				}
			}
		}
		break;

	case ES_Walk:
		{
			// Keep walking towards the Player.
			XMVECTOR playerVector = XMLoadFloat3(&Game::Get().GetPlayer()->GetPosition());
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			XMVECTOR direction = playerVector - positionVector;
			XMVECTOR directionNormalized = XMVector3Normalize(direction);
			m_facingAngle = XMConvertToDegrees((float)atan2(XMVectorGetY(directionNormalized), XMVectorGetX(directionNormalized))) + 180.0f;
			PlayAnimationForDirection("GoblinLeftRun.json", "GoblinDownRun.json", "GoblinRightRun.json", "GoblinUpRun.json");

			XMFLOAT3 position = GetPosition();
			float elapsedSeconds = (float)timer.GetElapsedSeconds();
			position.x += XMVectorGetX(directionNormalized) * MovementSpeed * elapsedSeconds;
			position.y += XMVectorGetY(directionNormalized) * MovementSpeed * elapsedSeconds;
			Move(position);

			// When we're close enough or if the Player moved too far, stop.
			XMVECTOR distance = XMVector3Length(direction);
			if (XMVectorGetX(distance) < MovementSpeed * elapsedSeconds ||
				XMVectorGetX(distance) >= AwareDistance)
			{
				m_state = ES_Idle;
				PlayAnimationForDirection("GoblinLeftIdle.json", "GoblinDownIdle.json", "GoblinRightIdle.json", "GoblinUpIdle.json");
			}

			// And if we're close enough to attack, start attacking them.
			if (XMVectorGetX(distance) < AttackDistance)
			{
				m_state = ES_Attack;
				PlayAnimationForDirection(
					"GoblinLeftAttack01.json", "GoblinDownAttack01.json", "GoblinRightAttack01.json", "GoblinUpAttack01.json", false);
			}
	}
		break;
	}

	WorldObject::Update(timer);
}
