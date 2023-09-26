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

		static const float AttackDistance;		// distance in world space
		static const float AwareDistance;		// distance in world space
		static const float MovementSpeed;		// pixels-per-second
	};
}
