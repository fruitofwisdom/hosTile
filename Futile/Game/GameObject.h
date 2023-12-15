#pragma once

#include "..\hosTile\hTFrameData.h"
#include "..\hosTile\hTRenderer.h"
#include "StepTimer.h"
#include <string>

// A GameObject represents any object that can be placed in the game, but doesn't necessarily have
// to have an associated sprite. For example, an enemy spawner.
namespace Futile
{
	class GameObject
	{
	public:
		GameObject(DirectX::XMFLOAT3 position);
		virtual ~GameObject() {}

		virtual void Update(const DX::StepTimer& timer) {}
		virtual void Render(hosTile::hTRenderer& renderer) {}

		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(DirectX::XMFLOAT3 position);

		std::string GetType() const;

		virtual hosTile::hTRegion GetCollision() const;
		virtual hosTile::hTRegion GetHitBox() const;
		virtual hosTile::hTRegion GetHurtBox() const;
		virtual void ReceiveHit(const GameObject* attacker);

	protected:
		std::string m_type;

	private:
		DirectX::XMFLOAT3 m_position;
	};
}
