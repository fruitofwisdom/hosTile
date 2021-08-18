#include "pch.h"
#include "hosTileSampleMain.h"

#include "DirectX\DirectXHelper.h"

using namespace Concurrency;
using namespace hosTile;
using namespace hosTileSample;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

// Loads and initializes application assets when the application is loaded.
hosTileSampleMain::hosTileSampleMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer->AddSprite(
		std::shared_ptr<hosTileSprite>(new hosTileSprite(
			m_deviceResources, L"Assets/NES - Final Fantasy - Castle Corneria.dds", DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)
		))
	);
	m_sceneRenderer->AddSprite(
		std::shared_ptr<hosTileSprite>(new hosTileSprite(
			m_deviceResources, L"Assets/NES - Final Fantasy - Warrior.dds", DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f)
		))
	);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

hosTileSampleMain::~hosTileSampleMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void hosTileSampleMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void hosTileSampleMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update();
		//m_fpsTextRenderer->Update(m_timer);
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
	// TODO: Replace this with your app's content rendering functions.
	m_sceneRenderer->Render();
	//m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void hosTileSampleMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	//m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void hosTileSampleMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	//m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
