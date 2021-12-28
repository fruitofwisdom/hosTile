#include "pch.h"
#include "Game.h"

#include <fstream>
#include "hosTile\hTException.h"
#include "hosTile\hTTileset.h"
#include "hosTile\Other\json.hpp"

using namespace hosTile;
using namespace hosTileSample;
using namespace nlohmann;
using namespace std;

const float Game::Scale = 2.0f;

Game::Game(hTRenderer* renderer)
{
	ifstream mapFile("futile_map.json");
	json mapJson;
	mapFile >> mapJson;
	string tilesetSource = mapJson["tilesets"][0]["source"];
	// the tileset is kept internally as a .tsx file, but exported as a .json
	tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

	auto deviceResources = renderer->GetDeviceResources();

	auto tileset = make_shared<hTTileset>(deviceResources, tilesetSource);

	try
	{
		m_map = make_shared<hTMap>(tileset, mapJson);
		m_map->SetScale(Scale);
		renderer->AddSprite(m_map);

		// TODO: Search through only objectgroups.
		json objects = mapJson["layers"][1]["objects"];
		for (json object : objects)
		{
			if (object["name"] == "player")
			{
				// Objects are placed in Tiled based on their bottom-left corner from the top-left
				// corner of the map. Translate that to an absolute position in our game's space.
				float x = (m_map->GetPosition().x - m_map->GetWidth() / 2 + object["x"] + tileset->GetTileWidth() / 2) * Scale;
				float y = (m_map->GetPosition().y + m_map->GetHeight() / 2 - object["y"] + tileset->GetTileHeight() / 2) * Scale;
				auto playerSprite = make_shared<hTTileSprite>(
					tileset, object["gid"], DirectX::XMFLOAT3(x, y, 0.0f));
				playerSprite->SetScale(Scale);
				m_player = std::make_unique<Player>(playerSprite);
				renderer->AddSprite(playerSprite);
			}
		}
	}
	catch (hTException&)
	{
		// TODO: Exception handling.
	}

	m_camera = std::make_unique<Camera>(renderer, m_player.get());
}

void Game::Update(const DX::StepTimer& timer)
{
	m_map->Update();
	m_player->Update(timer);
	m_camera->Update();
}
