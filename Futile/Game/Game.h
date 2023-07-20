#pragma once

#include "Camera.h"
#include "..\hosTile\hTFont.h"
#include "..\hosTile\hTMap.h"
#include "..\hosTile\hTRenderer.h"
#include "Player.h"
#include "StepTimer.h"
#include "TextBox.h"
#include "UI.h"

namespace Futile
{
	// The game instance.
	class Game
	{
	public:
		Game(hosTile::hTRenderer& renderer);
		virtual ~Game();

		Game(Game const&) = delete;
		Game& operator=(Game const&) = delete;

		// Game is a singleton.
		static Game& Get();

		void Update(const DX::StepTimer& timer);
		void Render();

		// The primary renderer used for the camera, UI, mouse, etc.
		hosTile::hTRenderer* GetRenderer() const;
		// The primary UI.
		UI& GetUI() const;

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
		std::unique_ptr<UI> m_ui;

		static const float Scale;
	};
}
