﻿#include "pch.h"
#include "App.h"

#include <fstream>
#include "..\hosTile\Other\json.hpp"
#include <ppltasks.h>

using namespace Futile;
using namespace nlohmann;
using namespace std;

using namespace Concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App();
}

// Default to a widescreen resolution that's generally lower than most monitor's.
const float App::PreferredWidth = 1280.0f;
const float App::PreferredHeight = 720.0f;

Platform::String^ App::s_version = L"unknown";

App::App()
:	m_windowClosed(false),
	m_windowVisible(true)
{
	ApplicationView::PreferredLaunchViewSize = Size(PreferredWidth, PreferredHeight);
	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;

	ifstream configFile("Futile.json");
	if (configFile.is_open())
	{
		json configJson;
		configFile >> configJson;

		if (configJson.contains("version"))
		{
			string version = configJson["version"];
			wstring wversion = wstring(version.begin(), version.end());
			s_version = ref new Platform::String(wversion.c_str());
		}
		if (configJson.contains("displayMode"))
		{
			string displayMode = configJson["displayMode"];
			if (displayMode == "fullscreen")
			{
				ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::FullScreen;
			}
			else if (displayMode == "maximized")
			{
				// NOTE: Maximized is not currently supported by UWP.
				ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::Maximized;
			}
			else if (displayMode == "windowed")
			{
				// Do nothing. Windowed is default.
			}
		}
		if (configJson.contains("startingMap"))
		{
			m_startingMap = configJson["startingMap"];
		}
	}
}

// The first method called when the IFrameworkView is being created.
void App::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = make_unique<DX::DeviceResources>();
	m_input = make_unique<Input>();
}

// Called when the CoreWindow object is created (or re-created).
void App::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDisplayContentsInvalidated);

	m_deviceResources->SetWindow(window);
	m_input->SetWindow(window);
	m_input->SetDpi(currentDisplayInformation->LogicalDpi);
}

// Initializes scene resources, or loads a previously saved app state.
void App::Load(Platform::String^ entryPoint)
{
	if (m_main == nullptr)
	{
		m_main = make_unique<FutileMain>(m_deviceResources.get(), m_startingMap);
	}
}

// This method is called after the window becomes active.
void App::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			m_input->Update();

			// Handle the classic Alt+Enter fullscreen toggle combination.
			if ((m_input->GetKeyboardState().LeftAlt || m_input->GetKeyboardState().RightAlt) &&
				m_input->GetKeyboardTracker().IsKeyPressed(DirectX::Keyboard::Enter))
			{
				auto av = ApplicationView::GetForCurrentView();
				if (!av->IsFullScreenMode)
				{
					if (av->TryEnterFullScreenMode())
					{
						av->FullScreenSystemOverlayMode = FullScreenSystemOverlayMode::Minimal;
					}
				}
				else
				{
					av->ExitFullScreenMode();
					av->FullScreenSystemOverlayMode = FullScreenSystemOverlayMode::Standard;
				}
			}

			m_main->Update();

			if (m_main->Render())
			{
				m_deviceResources->Present();
			}
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void App::Uninitialize()
{
}

Platform::String^ App::GetVersion()
{
	return s_version;
}

// Application lifecycle event handlers.

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		m_deviceResources->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
}

// Window event handlers.

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_deviceResources->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	
	if (m_main != nullptr)
	{
		m_main->CreateWindowSizeDependentResources();
	}
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
	m_input->SetDpi(sender->LogicalDpi);
}

void App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->ValidateDevice();
}
