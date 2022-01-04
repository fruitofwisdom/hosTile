#pragma once

#include "GameObject.h"
#include "hosTile\hTRenderer.h"

namespace hosTileSample
{
	class Camera
	{
	public:
		Camera(hosTile::hTRenderer* renderer, const GameObject* focus);
		virtual ~Camera() {}

		void Update();

	private:
		hosTile::hTRenderer* m_renderer;
		const GameObject* m_focus;
	};
}
