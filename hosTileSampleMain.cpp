#include "pch.h"
#include "hosTileSampleMain.h"

#include "DirectX\DirectXHelper.h"

using namespace Concurrency;
using namespace hosTileSample;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

// Loads and initializes application assets when the application is loaded.
hosTileSampleMain::hosTileSampleMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	m_renderer = std::shared_ptr<hosTile::hosTileRenderer>(new hosTile::hosTileRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	m_game = std::unique_ptr<Game>(new Game(m_renderer));
}

hosTileSampleMain::~hosTileSampleMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void hosTileSampleMain::CreateWindowSizeDependentResources() 
{
	m_renderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void hosTileSampleMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		m_game->Update(m_timer);
		m_renderer->Update();
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool hosTileSampleMain::Render() 
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
	m_renderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void hosTileSampleMain::OnDeviceLost()
{
	m_renderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void hosTileSampleMain::OnDeviceRestored()
{
	m_renderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
