#include "pch.h"
#include "Game.h"

#include "App.h"
#include "..\hosTile\Other\json.hpp"

using namespace DirectX;
using namespace hosTile;
using namespace Futile;
using namespace nlohmann;
using namespace std;

const float Game::Scale = 4.0f;

static Game* s_game = nullptr;

Game::Game(hTRenderer& renderer, string startingMap)
:	m_gameState(GS_Intro),
	m_renderer(&renderer)
{
	if (s_game)
	{
		throw logic_error("Game is a singleton!");
	}
	s_game = this;

	m_renderer->SetScale(Scale);

	m_level = make_unique<Level>(startingMap);
	m_camera = make_unique<Camera>(*m_renderer, m_level->GetPlayer());

	// TODO: The text box and UI should have its own tileset.
	m_textBox = make_unique<TextBox>(
		"futile_tileset.json", "futile_textbox.json", "futile_font.dds",
		L"Welcome to Futile - a demo game for hosTile!\n\nPress space to play!");
	m_ui = make_unique<UI>("futile_font.dds", App::GetVersion()->Data());
}

Game::~Game()
{
	s_game = nullptr;
}

Game& Game::Get()
{
	if (s_game == nullptr)
	{
		throw logic_error("Game not yet created!");
	}
	return *s_game;
}

void Game::Update(const DX::StepTimer& timer)
{
	switch (m_gameState)
	{
	case GS_Intro:
		m_camera->Update();

		XMFLOAT3 textBoxPosition = m_renderer->ScreenToWorldPosition(
			(int)(m_renderer->GetDeviceResources()->GetLogicalSize().Width / 2),
			(int)(m_textBox->GetHeight() / 2.0f * m_renderer->GetScale()));
		m_textBox->SetPosition(textBoxPosition);

		// Press Enter, Space, or left-click in the text box to close it.
		XMFLOAT3 mousePosition = m_renderer->ScreenToWorldPosition(
			Input::Get().GetMouseState().x, Input::Get().GetMouseState().y);
		if (Input::Get().GetKeyboardTracker().IsKeyPressed(Keyboard::Keys::Enter) ||
			Input::Get().GetKeyboardTracker().IsKeyPressed(Keyboard::Keys::Space) ||
			(Input::Get().GetMouseTracker().leftButton == Mouse::ButtonStateTracker::PRESSED &&
				m_textBox->Contains(mousePosition.x, mousePosition.y)))
		{
			m_gameState = GS_Playing;
			m_textBox.release();
		}
	break;

	case GS_Playing:
		m_level->Update(timer);
		m_camera->Update();
		break;
	}

	// An example of printing debug text.
	/*
	wstringstream debugText;
	debugText << fixed << setprecision(3);
	debugText << "player: " << m_level->GetPlayer()->GetPosition().x << ", " << m_level->GetPlayer()->GetPosition().y;
	m_ui->SetDebugText(debugText.str().c_str());
	*/
	m_ui->Update();
}

void Game::Render()
{
	m_level->Render();
	if (m_textBox)
	{
		m_textBox->Render();
	}
	m_ui->Render();
}

const Player* Game::GetPlayer() const
{
	return m_level->GetPlayer();
}

hTRenderer& Game::GetRenderer() const
{
	return *m_renderer;
}

UI& Game::GetUI() const
{
	return *m_ui;
}
