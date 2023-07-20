#include "pch.h"
#include "Input.h"

#include <stdexcept>

using namespace DirectX;
using namespace Futile;
using namespace std;

static Input* s_input = nullptr;

Input::Input()
{
	if (s_input)
	{
		throw logic_error("Input is a singleton!");
	}
	s_input = this;

	m_keyboard = make_unique<DirectX::Keyboard>();
	m_mouse = make_unique<DirectX::Mouse>();
}

Input::~Input()
{
	s_input = nullptr;
}

Input& Input::Get()
{
	if (s_input == nullptr)
	{
		throw logic_error("Input not yet created!");
	}
	return *s_input;
}

void Input::SetWindow(Windows::UI::Core::CoreWindow^ window)
{
	m_keyboard->SetWindow(window);
	m_mouse->SetWindow(window);
}

void Input::SetDpi(float dpi)
{
	m_mouse->SetDpi(dpi);
}

void Input::Update()
{
	auto keyboardState = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardState);
	auto mouseState = m_mouse->GetState();
	m_mouseTracker.Update(mouseState);
}

Keyboard::State Input::GetKeyboardState() const
{
	return m_keyboard->GetState();
}

Keyboard::KeyboardStateTracker Input::GetKeyboardTracker() const
{
	return m_keyboardTracker;
}

Mouse::State Input::GetMouseState() const
{
	return m_mouse->GetState();
}

Mouse::ButtonStateTracker Input::GetMouseTracker() const
{
	return m_mouseTracker;
}
