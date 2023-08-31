#include "pch.h"
#include "Level.h"

#include <fstream>
#include "Game.h"
#include "..\hosTile\hTException.h"
#include "..\hosTile\hTRenderer.h"
#include "..\hosTile\Other\json.hpp"
#include "Utilities.h"

using namespace Futile;
using namespace hosTile;
using namespace nlohmann;
using namespace std;

Level::Level(string levelFilename)
:	m_loaded(false)
{
	ifstream levelFile(levelFilename);
	if (levelFile.is_open())
	{
		try
		{
			json levelJson;
			levelFile >> levelJson;
			string tilesetSource = levelJson["tilesets"][0]["source"];
			// the tileset is kept internally as a .tsx file, but exported as a .json
			tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

			DX::DeviceResources* deviceResources = Game::Get().GetRenderer().GetDeviceResources();
			m_tileset = make_unique<hTTileset>(deviceResources, tilesetSource);

			m_map = make_unique<hTMap>(*m_tileset, levelJson);
			m_map->SetScale(Game::Scale);

			if (levelJson.contains("backgroundcolor"))
			{
				string backgroundColor = levelJson["backgroundcolor"];
				Game::Get().GetRenderer().SetClearColor(Utilities::HexColorCodeToVector(backgroundColor));
			}

			json layers = levelJson["layers"];
			for (json layer : layers)
			{
				// Load game objects from all object layers.
				if (layer.contains("objects"))
				{
					json objects = layer["objects"];
					for (json object : objects)
					{
						if (object["name"] == "player")
						{
							// Objects are placed in Tiled based on their bottom-left corner from the top-left
							// corner of the map. Translate that to an absolute position in our game's space.
							float x = (m_map->GetPosition().x
								- m_map->GetWidth() / 2.0f + object["x"]
								+ m_tileset->GetTileWidth() / 2.0f) * Game::Scale;
							float y = (m_map->GetPosition().y
								+ m_map->GetHeight() / 2.0f - object["y"]
								+ m_tileset->GetTileHeight() / 2.0f) * Game::Scale;
							m_player = make_unique<Player>();
							m_player->SetPosition(DirectX::XMFLOAT3(x, y, 0.0f));
							m_player->GetSprite()->SetScale(Game::Scale);
						}

						// TODO: Additional game objects.
					}
				}
			}

			// A Player is required to be loaded.
			if (m_player)
			{
				m_loaded = true;
			}
		}
		catch (hTException&)
		{
			wstringstream debugText;
			debugText << "Level \"" << levelFilename.c_str() << "\" failed to load!";
			Game::Get().GetUI().SetDebugText(debugText.str().c_str());
		}
	}
}

void Level::Update(const DX::StepTimer& timer)
{
	if (m_loaded)
	{
		m_player->Update(timer);
	}
}

void Level::Render()
{
	if (m_loaded)
	{
		m_map->Render(Game::Get().GetRenderer());
		m_player->Render(Game::Get().GetRenderer());
	}
}

Player& Level::GetPlayer()
{
	return *m_player.get();
}

bool Level::IsLoaded() const
{
	return m_loaded;
}
