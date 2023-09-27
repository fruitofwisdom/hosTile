#pragma once

#include "WorldObject.h"

namespace Futile
{
	class Enemy : public WorldObject
	{
	public:
		Enemy(DirectX::XMFLOAT3 position, float scale = 1.0f);

		void Update(const DX::StepTimer& timer);

	private:
		enum EnemyState
		{
			ES_Attack,
			ES_Idle,
			ES_Walk
		};

		EnemyState m_state;

		DirectX::XMVECTOR m_walkingTarget;
		float m_timeSinceLastAttack;		// seconds

		static const float AttackCooldown;		// seconds
		static const float AttackDistanceFar;		// distance in world space
		static const float AttackDistanceNear;		// distance in world space
		static const float AwareDistance;		// distance in world space
		static const float MovementSpeed;		// pixels-per-second
	};
}
