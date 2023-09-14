#pragma once

#include "Camera.h"
#include "..\hosTile\hTFont.h"
#include "..\hosTile\hTRenderer.h"
#include "Level.h"
#include "StepTimer.h"
#include "TextBox.h"
#include "UI.h"

namespace Futile
{
	// The game instance.
	class Game
	{
	public:
		Game(hosTile::hTRenderer& renderer, std::string startingMap);
		virtual ~Game();

		Game(Game const&) = delete;
		Game& operator=(Game const&) = delete;

		// Game is a singleton.
		static Game& Get();

		void Update(const DX::StepTimer& timer);
		void Render();

		// The primary renderer used for the camera, UI, mouse, etc.
		hosTile::hTRenderer& GetRenderer() const;
		// The primary UI.
		UI& GetUI() const;

		static const float Scale;

	private:
		enum GameState
		{
			GS_Intro,
			GS_Playing
		};

		GameState m_gameState;

		hosTile::hTRenderer* m_renderer;

		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<Level> m_level;
		std::unique_ptr<TextBox> m_textBox;
		std::unique_ptr<UI> m_ui;
	};
}
