﻿#include "pch.h"
#include "FutileMain.h"

#include "..\hosTile\Other\DirectXHelper.h"

using namespace Concurrency;
using namespace Futile;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

// Loads and initializes application assets when the application is loaded.
FutileMain::FutileMain(DX::DeviceResources* deviceResources)
:	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	m_renderer = std::make_unique<hosTile::hTRenderer>(m_deviceResources);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	m_game = std::make_unique<Game>(*m_renderer.get());
}

FutileMain::~FutileMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void FutileMain::CreateWindowSizeDependentResources()
{
	m_renderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void FutileMain::Update()
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		m_game->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool FutileMain::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	m_game->Render();
	m_renderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void FutileMain::OnDeviceLost()
{
	m_renderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void FutileMain::OnDeviceRestored()
{
	m_renderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}