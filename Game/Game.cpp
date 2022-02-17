#include "pch.h"
#include "Game.h"

#include <fstream>
#include "hosTile\hTException.h"
#include "hosTile\hTTileset.h"
#include "hosTile\Other\json.hpp"
#include "Keyboard.h"

using namespace hosTile;
using namespace hosTileSample;
using namespace nlohmann;
using namespace std;

const float Game::Scale = 5.0f;

Game::Game(hTRenderer& renderer)
:	m_gameState(GS_Intro),
	m_renderer(&renderer)
{
	ifstream mapFile("futile_map.json");
	json mapJson;
	mapFile >> mapJson;
	string tilesetSource = mapJson["tilesets"][0]["source"];
	// the tileset is kept internally as a .tsx file, but exported as a .json
	tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

	DX::DeviceResources* deviceResources = m_renderer->GetDeviceResources();
	m_tileset = make_unique<hTTileset>(deviceResources, tilesetSource);

	const wchar_t* fontDescription = L"abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890.:,;\'\"(!?)+-*/= ";
	m_font = make_unique<hTFont>(deviceResources, "futile_font.dds", fontDescription);

	try
	{
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
					m_tileset.get(), object["gid"], DirectX::XMFLOAT3(x, y, 0.0f));
				playerSprite->SetScale(Scale);
				m_player = make_unique<Player>(move(playerSprite));
			}
		}

		m_textBox = make_unique<TextBox>(
			m_tileset.get(), "futile_textbox.json",
			m_font.get(), L"Welcome to Futile - a demo game for hosTile!\n\nPress any key to play!");
		m_textBox->SetScale(Scale);
	}
	catch (hTException& exception)
	{
		// Any hosTile exceptions can be handled generically.
		hTException::HandleException(exception);
	}

	m_camera = make_unique<Camera>(*m_renderer, *m_player.get());
}

void Game::Update(const DX::StepTimer& timer)
{
	if (m_gameState == GS_Playing)
	{
		m_player->Update(timer);
	}
	m_camera->Update();
	if (m_gameState == GS_Intro)
	{
		DirectX::XMFLOAT3 textBoxPosition = m_renderer->ScreenToWorldPosition(
			(unsigned int)(m_renderer->GetDeviceResources()->GetLogicalSize().Width / 2),
			(unsigned int)(m_textBox->GetHeight() / 2.0f));
		m_textBox->SetPosition(textBoxPosition);

		auto kb = DirectX::Keyboard::Get().GetState();
		if (kb.Enter || kb.Space)
		{
			m_gameState = GS_Playing;
			m_textBox.release();
		}
	}
}

void Game::Render()
{
	m_map->Render(*m_renderer);
	m_player->Render(*m_renderer);
	if (m_textBox != nullptr)
	{
		m_textBox->Render(*m_renderer);
	}
}
