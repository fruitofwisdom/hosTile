#pragma once

#include "GameObject.h"
#include "hosTile\hosTileRenderer.h"

namespace hosTileSample
{
	class Camera
	{
	public:
		Camera(std::shared_ptr<hosTile::hosTileRenderer> renderer, std::shared_ptr<GameObject> focus);
		virtual ~Camera() {}

		void Update();

	private:
		std::shared_ptr<hosTile::hosTileRenderer> m_renderer;
		std::shared_ptr<GameObject> m_focus;
	};
}
