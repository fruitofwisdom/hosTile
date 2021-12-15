#include "pch.h"
#include "Game.h"

#include <fstream>
#include "hosTile\hTException.h"
#include "hosTile\hTMap.h"
#include "hosTile\hTTileset.h"
#include "Other\json.hpp"

using namespace hosTile;
using namespace hosTileSample;
using namespace std;

Game::Game(std::shared_ptr<hTRenderer> renderer)
{
	ifstream mapFile("futile_map.json");
	nlohmann::json mapJson;
	mapFile >> mapJson;
	string tilesetSource = mapJson["tilesets"][0]["source"];
	// the tileset is kept internally as a .tsx file, but exported as a .json
	tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

	auto deviceResources = renderer->GetDeviceResources();

	auto tileset = make_shared<hTTileset>(deviceResources, tilesetSource);

	try
	{
		auto map = make_shared<hTMap>(tileset, mapJson);
		map->SetScale(2.0f);
		map->Update();
		renderer->AddSprite(map);
	}
	catch (hTException& exception)
	{
		// TODO: Exception handling.
	}

	// TODO: Assumes the first object is the player. Go find the object "player".
	unsigned int playerTileNum = mapJson["layers"][1]["objects"][0]["gid"] - 1;
	auto playerSprite = make_shared<hTTileSprite>(
		tileset, playerTileNum,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	playerSprite->SetScale(2.0f);
	m_player = std::make_shared<Player>(playerSprite);
	renderer->AddSprite(playerSprite);

	m_camera = std::make_unique<Camera>(renderer, m_player);
}

void Game::Update(const DX::StepTimer& timer)
{
	m_player->Update(timer);
	m_camera->Update();
}
