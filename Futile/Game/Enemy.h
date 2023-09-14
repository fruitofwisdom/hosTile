#pragma once

#include "GameObject.h"

namespace Futile
{
	class Enemy : public GameObject
	{
	public:
		Enemy(DirectX::XMFLOAT3 position, float scale = 1.0f);

		void Update(const DX::StepTimer& timer);
	};
}
