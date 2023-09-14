#include "pch.h"
#include "Level.h"

#include <fstream>
#include "Game.h"
#include "Enemy.h"
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
						// Objects are placed in Tiled as rectangles with an x, y at their top-left from the top-left
						// of the map. Translate that to an absolute position in our game's space, where sprites and
						// the map are positioned at their center.
						float x =
							(m_map->GetPosition().x - m_map->GetWidth() / 2.0f +
							object["x"] + object["width"] / 2.0f) * Game::Scale;
						float y =
							(m_map->GetPosition().y + m_map->GetHeight() / 2.0f -
							object["y"] - object["height"] / 2.0f) * Game::Scale;
						DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(x, y, 0.0f);

						if (object["type"] == "Enemy")
						{
							unique_ptr<Enemy> newEnemy = make_unique<Enemy>(position, Game::Scale);
							m_gameObjects.push_back(move(newEnemy));
						}
						else if (object["type"] == "Player")
						{
							m_player = make_unique<Player>(position, Game::Scale);
						}
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
		for (unique_ptr<GameObject>& gameObject : m_gameObjects)
		{
			gameObject->Update(timer);
		}
	}
}

void Level::Render()
{
	if (m_loaded)
	{
		m_map->Render(Game::Get().GetRenderer());
		m_player->Render(Game::Get().GetRenderer());
		for (unique_ptr<GameObject>& gameObject : m_gameObjects)
		{
			gameObject->Render(Game::Get().GetRenderer());
		}
	}
}

Player& Level::GetPlayer() const
{
	return *m_player.get();
}

bool Level::IsLoaded() const
{
	return m_loaded;
}
