#pragma once

#include "DirectX\DeviceResources.h"
#include "DirectX\StepTimer.h"
#include "Game\Game.h"
#include "hosTile\hosTileRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace hosTileSample
{
	class hosTileSampleMain : public DX::IDeviceNotify
	{
	public:
		hosTileSampleMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~hosTileSampleMain();

		void CreateWindowSizeDependentResources();

		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// The hosTile renderer.
		std::shared_ptr<hosTile::hosTileRenderer> m_renderer;

		// Game loop timer.
		DX::StepTimer m_timer;

		// The game instance.
		std::unique_ptr<Game> m_game;
	};
}