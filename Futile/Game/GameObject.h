#pragma once

#include "..\hosTile\hTSprite.h"
#include "StepTimer.h"

namespace Futile
{
	class GameObject
	{
	public:
		GameObject(std::unique_ptr<hosTile::hTSprite> sprite);
		virtual ~GameObject() {}

		virtual void Update(const DX::StepTimer& timer) {}

		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(DirectX::XMFLOAT3 position);

		hosTile::hTSprite* GetSprite() const;

	protected:
		std::unique_ptr<hosTile::hTSprite> m_sprite;
	};
}
