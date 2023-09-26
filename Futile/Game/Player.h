#pragma once

#include "WorldObject.h"

namespace Futile
{
	class Player : public WorldObject
	{
	public:
		Player(DirectX::XMFLOAT3 position, float scale = 1.0f);

		void Update(const DX::StepTimer& timer);

	private:
		enum PlayerState
		{
			PS_Attack,
			PS_Idle,
			PS_Walk
		};

		PlayerState m_state;

		DirectX::XMFLOAT3 m_walkingTarget;

		static const float MovementSpeed;		// pixels-per-second
	};
}
