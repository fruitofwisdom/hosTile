#pragma once

#include "GameObject.h"
#include "hosTile\hTRenderer.h"

namespace hosTileSample
{
	class Camera
	{
	public:
		Camera(std::shared_ptr<hosTile::hTRenderer> renderer, std::shared_ptr<GameObject> focus);
		virtual ~Camera() {}

		void Update();

	private:
		std::shared_ptr<hosTile::hTRenderer> m_renderer;
		std::shared_ptr<GameObject> m_focus;
	};
}
