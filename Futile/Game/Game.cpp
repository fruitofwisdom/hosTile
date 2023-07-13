#include "pch.h"
#include "Game.h"

#include "App.h"
#include <fstream>
#include "..\hosTile\hTException.h"
#include "..\hosTile\hTTileset.h"
#include "..\hosTile\Other\json.hpp"
#include "Keyboard.h"

using namespace DirectX;
using namespace hosTile;
using namespace Futile;
using namespace nlohmann;
using namespace std;

const float Game::Scale = 5.0f;

Game::Game(hTRenderer& renderer)
:	m_gameState(GS_Intro),
	m_renderer(&renderer)
{
	ifstream mapFile("futile_map.json");
	if (mapFile.is_open())
	{
		try
		{
			json mapJson;
			mapFile >> mapJson;
			string tilesetSource = mapJson["tilesets"][0]["source"];
			// the tileset is kept internally as a .tsx file, but exported as a .json
			tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

			DX::DeviceResources* deviceResources = m_renderer->GetDeviceResources();
			m_tileset = make_unique<hTTileset>(deviceResources, tilesetSource);

			// TODO: This should be a part of "futile_font.json".
			const wchar_t* fontDescription = L"abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890.:,;\'\"(!?)+-*/= ";
			m_font = make_unique<hTFont>(deviceResources, "futile_font.dds", fontDescription);

			m_map = make_unique<hTMap>(m_tileset.get(), mapJson);
			m_map->SetScale(Scale);

			// TODO: Search through only objectgroups.
			json objects = mapJson["layers"][1]["objects"];
			for (json object : objects)
			{
				if (object["name"] == "player")
				{
					// Objects are placed in Tiled based on their bottom-left corner from the top-left
					// corner of the map. Translate that to an absolute position in our game's space.
					float x = (m_map->GetPosition().x
						- m_map->GetWidth() / 2.0f + object["x"]
						+ m_tileset->GetTileWidth() / 2.0f) * Scale;
					float y = (m_map->GetPosition().y
						+ m_map->GetHeight() / 2.0f - object["y"]
						+ m_tileset->GetTileHeight() / 2.0f) * Scale;
					auto playerSprite = make_unique<hTTileSprite>(
						m_tileset.get(), object["gid"], XMFLOAT3(x, y, 0.0f));
					playerSprite->SetScale(Scale);
					m_player = make_unique<Player>(move(playerSprite));
				}
			}

			m_textBox = make_unique<TextBox>(
				m_tileset.get(), "futile_textbox.json",
				m_font.get(), L"Welcome to Futile - a demo game for hosTile!\n\nPress space to play!");
			m_textBox->SetScale(Scale);
		}
		catch (hTException& exception)
		{
			// Any hosTile exceptions can be handled generically.
			hTException::HandleException(exception);
		}

		m_camera = make_unique<Camera>(*m_renderer, *m_player.get());

		m_ui = make_unique<UI>(*m_renderer, m_font.get(), App::GetVersion()->Data());
		m_ui->SetScale(Scale);
	}
}

void Game::Update(const DX::StepTimer& timer)
{
	if (m_gameState == GS_Playing)
	{
		if (m_player)
		{
			m_player->Update(timer);
		}
	}
	m_camera->Update();
	if (m_gameState == GS_Intro)
	{
		XMFLOAT3 textBoxPosition = m_renderer->ScreenToWorldPosition(
			(int)(m_renderer->GetDeviceResources()->GetLogicalSize().Width / 2),
			(int)(m_textBox->GetHeight() / 2.0f));
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
	}

	// An example of printing debug text.
	/*
	wstringstream debugText;
	debugText << fixed << setprecision(3);
	debugText << "player: " << m_player->GetPosition().x << ", " << m_player->GetPosition().y;
	m_ui->SetDebugText(debugText.str().c_str());
	*/
	m_ui->Update();
}

void Game::Render()
{
	m_map->Render(*m_renderer);
	m_player->Render(*m_renderer);
	if (m_textBox != nullptr)
	{
		m_textBox->Render(*m_renderer);
	}
	m_ui->Render();
}
