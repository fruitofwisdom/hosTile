#pragma once

#include "GameObject.h"
#include "..\hosTile\hTRenderer.h"

namespace Futile
{
	class Camera
	{
	public:
		Camera(hosTile::hTRenderer& renderer, const GameObject* focus);
		virtual ~Camera() {}

		void Update();

		DirectX::XMFLOAT3 GetPosition() const;

	private:
		hosTile::hTRenderer* m_renderer;
		const GameObject* m_focus;
	};
}
