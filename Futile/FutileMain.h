#pragma once

#include "Game\Game.h"
#include "Game\StepTimer.h"
#include "..\hosTile\hTRenderer.h"
#include "..\hosTile\Other\DeviceResources.h"

// Renders Direct2D and 3D content on the screen.
namespace Futile
{
	class FutileMain : public DX::IDeviceNotify
	{
	public:
		FutileMain(DX::DeviceResources* deviceResources, std::string startingMap);
		~FutileMain();

		void CreateWindowSizeDependentResources();

		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		DX::DeviceResources* m_deviceResources;

		// The hosTile renderer.
		std::unique_ptr<hosTile::hTRenderer> m_renderer;

		// Game loop timer.
		DX::StepTimer m_timer;

		// The game instance.
		std::unique_ptr<Game> m_game;
	};
}