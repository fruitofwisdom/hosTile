#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace Futile
{
	class Input
	{
	public:
		Input();
		virtual ~Input();

		Input(Input const&) = delete;
		Input& operator=(Input const&) = delete;

		static Input& Get();

		void SetWindow(Windows::UI::Core::CoreWindow^ window);
		void SetDpi(float dpi);

		void Update();

		DirectX::Keyboard::State GetKeyboardState() const;
		DirectX::Keyboard::KeyboardStateTracker GetKeyboardTracker() const;
		DirectX::Mouse::State GetMouseState() const;
		DirectX::Mouse::ButtonStateTracker GetMouseTracker() const;

	private:
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
		std::unique_ptr<DirectX::Mouse> m_mouse;
		DirectX::Mouse::ButtonStateTracker m_mouseTracker;
		// TODO: Game pad support?
	};
}
