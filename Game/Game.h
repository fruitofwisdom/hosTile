#pragma once

#include "Camera.h"
#include "hosTile\hTFont.h"
#include "hosTile\hTMap.h"
#include "hosTile\hTRenderer.h"
#include "Player.h"
#include "StepTimer.h"
#include "TextBox.h"

// The game instance.
namespace hosTileSample
{
	class Game final
	{
	public:
		Game(hosTile::hTRenderer& renderer);

		void Update(const DX::StepTimer& timer);
		void Render();

	private:
		enum GameState
		{
			GS_Intro,
			GS_Playing
		};

		GameState m_gameState;

		hosTile::hTRenderer* m_renderer;

		std::unique_ptr<hosTile::hTFont> m_font;
		std::unique_ptr<hosTile::hTMap> m_map;
		std::unique_ptr<hosTile::hTTileset> m_tileset;

		std::unique_ptr<Player> m_player;
		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<TextBox> m_textBox;

		static const float Scale;
	};
}
