﻿#pragma once

#include "Common\StepTimer.h"
#include "hosTile\DeviceResources.h"
#include "hosTile\hosTileSprite.h"
#include "hosTile\Sample3DSceneRenderer.h"

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

		// TODO: Replace with your own content renderers.
		std::unique_ptr<hosTile::Sample3DSceneRenderer> m_sceneRenderer;

		// Game loop timer.
		DX::StepTimer m_timer;
	};
}