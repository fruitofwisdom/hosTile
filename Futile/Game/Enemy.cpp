#include "pch.h"
#include "Enemy.h"

#include "Game.h"
#include "Utilities.h"

using namespace DirectX;
using namespace Futile;

const float Enemy::AttackCooldown = 2.5f;		// seconds
const float Enemy::AttackDistanceFar = 25.0f;		// distance in world space
const float Enemy::AttackDistanceNear = 20.0f;		// distance in world space
const float Enemy::AwareDistance = 125.0f;		// distance in world space
const float Enemy::MovementSpeed = 32.0f;		// pixels-per-second

Enemy::Enemy(XMFLOAT3 position, float scale)
:	WorldObject("GoblinDownIdle.json", position, scale),
	m_state(ES_Idle),
	m_timeSinceLastAttack(AttackCooldown)
{
	m_type = "enemy";
	m_maxHP = 100;
	m_currentHP = m_maxHP;
	m_strength = 15;
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
				m_timeSinceLastAttack = 0.0f;
				PlayAnimationForDirection("GoblinLeftIdle.json", "GoblinDownIdle.json", "GoblinRightIdle.json", "GoblinUpIdle.json");
			}
		}
		break;

	case ES_Death:
		{
			if (m_sprite->AnimationDone())
			{
				// TODO: Go away somehow.
			}
		}
		break;

	case ES_Hurt:
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
			// If the player is dead, do nothing.
			if (Game::Get().GetPlayer()->GetCurrentHP() <= 0)
			{
				break;
			}

			// Only act when we're within aware distance of the player.
			XMVECTOR playerVector = XMLoadFloat3(&Game::Get().GetPlayer()->GetPosition());
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			float distanceToPlayer = XMVectorGetX(XMVector3Length(playerVector - positionVector));
			if (distanceToPlayer < AwareDistance)
			{
				float elapsedSeconds = (float)timer.GetElapsedSeconds();
				m_timeSinceLastAttack += elapsedSeconds;

				if (distanceToPlayer > AttackDistanceNear && distanceToPlayer < AttackDistanceFar)
				{
					if (m_timeSinceLastAttack > AttackCooldown)
					{
						m_state = ES_Attack;
						XMVECTOR directionNormalized = XMVector3Normalize(playerVector - positionVector);
						m_facingAngle = Utilities::GetAngleTo(directionNormalized);
						PlayAnimationForDirection(
							"GoblinLeftAttack01.json", "GoblinDownAttack01.json", "GoblinRightAttack01.json", "GoblinUpAttack01.json", false);
					}
				}
				else
				{
					m_state = ES_Walk;

					// Note that we want to walk just close enough to the player.
					XMVECTOR directionNormalized = XMVector3Normalize(playerVector - positionVector);
					m_walkingTarget = playerVector - directionNormalized * (AttackDistanceFar + AttackDistanceNear) / 2.0f;
					directionNormalized = XMVector3Normalize(m_walkingTarget - positionVector);
					m_facingAngle = Utilities::GetAngleTo(directionNormalized);
					PlayAnimationForDirection("GoblinLeftRun.json", "GoblinDownRun.json", "GoblinRightRun.json", "GoblinUpRun.json");
				}
			}
		}
		break;

	case ES_Walk:
		{
			// Only act when we're within aware distance of the player and the player is alive.
			XMVECTOR playerVector = XMLoadFloat3(&Game::Get().GetPlayer()->GetPosition());
			XMVECTOR positionVector = XMLoadFloat3(&GetPosition());
			float distanceToPlayer = XMVectorGetX(XMVector3Length(playerVector - positionVector));
			if (distanceToPlayer < AwareDistance && Game::Get().GetPlayer()->GetCurrentHP() > 0)
			{
				float elapsedSeconds = (float)timer.GetElapsedSeconds();
				m_timeSinceLastAttack += elapsedSeconds;

				// Note that we want to walk just close enough to the player.
				XMVECTOR directionNormalized = XMVector3Normalize(playerVector - positionVector);
				m_walkingTarget = playerVector - directionNormalized * (AttackDistanceFar + AttackDistanceNear) / 2.0f;
				directionNormalized = XMVector3Normalize(m_walkingTarget - positionVector);
				m_facingAngle = Utilities::GetAngleTo(directionNormalized);
				PlayAnimationForDirection("GoblinLeftRun.json", "GoblinDownRun.json", "GoblinRightRun.json", "GoblinUpRun.json");

				XMFLOAT3 position = GetPosition();
				position.x += XMVectorGetX(directionNormalized) * MovementSpeed * elapsedSeconds;
				position.y += XMVectorGetY(directionNormalized) * MovementSpeed * elapsedSeconds;
				Move(position);

				// Stop when we're in range.
				if (distanceToPlayer > AttackDistanceNear && distanceToPlayer < AttackDistanceFar)
				{
					if (m_timeSinceLastAttack > AttackCooldown)
					{
						m_state = ES_Attack;
						directionNormalized = XMVector3Normalize(playerVector - positionVector);
						m_facingAngle = Utilities::GetAngleTo(directionNormalized);
						PlayAnimationForDirection(
							"GoblinLeftAttack01.json", "GoblinDownAttack01.json", "GoblinRightAttack01.json", "GoblinUpAttack01.json", false);
					}
					else
					{
						m_state = ES_Idle;
						directionNormalized = XMVector3Normalize(playerVector - positionVector);
						m_facingAngle = Utilities::GetAngleTo(directionNormalized);
						PlayAnimationForDirection("GoblinLeftIdle.json", "GoblinDownIdle.json", "GoblinRightIdle.json", "GoblinUpIdle.json");
					}
				}
			}
			else
			{
				m_state = ES_Idle;
				PlayAnimationForDirection("GoblinLeftIdle.json", "GoblinDownIdle.json", "GoblinRightIdle.json", "GoblinUpIdle.json");
			}
		}
		break;
	}

	WorldObject::Update(timer);
}

void Enemy::ReceiveHit(const GameObject* attacker)
{
	const Player* player = dynamic_cast<const Player*>(attacker);
	if (player)
	{
		m_currentHP -= player->GetStrength();
		if (m_currentHP > 0)
		{
			m_state = ES_Hurt;
			PlayAnimationForDirection(
				"GoblinLeftHurt.json", "GoblinDownHurt.json", "GoblinRightHurt.json", "GoblinUpHurt.json", false);
		}
		else
		{
			m_state = ES_Death;
			PlayAnimationForDirection(
				"GoblinLeftDeath.json", "GoblinDownDeath.json", "GoblinRightDeath.json", "GoblinUpDeath.json", false);
		}
	}
}
