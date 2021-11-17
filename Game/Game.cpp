#include "pch.h"
#include "Game.h"

#include <fstream>
#include "hosTile\hosTileSprite.h"
#include "hosTile\hosTileTileset.h"
#include "hosTile\hosTileTileSprite.h"
#include "Other\json.hpp"

using namespace hosTile;
using namespace hosTileSample;
using namespace std;

Game::Game(std::shared_ptr<hosTileRenderer> renderer)
{
	ifstream mapFile("futile_map.json");
	nlohmann::json mapJson;
	mapFile >> mapJson;
	string tilesetSource = mapJson["tilesets"][0]["source"];
	// the tileset is kept internally as a .tsx file, but exported as a .json
	tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

	auto deviceResources = renderer->GetDeviceResources();

	auto tileset = make_shared<hosTileTileset>(deviceResources, tilesetSource);

	// TODO: Create a proper map class.
	auto map = make_shared<hosTileSprite>(deviceResources, tileset->GetImageFilename());
	map->SetScale(2.0f);
	renderer->AddSprite(map);

	/*
	// TODO: Handle tiles in a simple way instead of "sub-sprites"?
	auto playerSprite = make_shared<hosTile::hosTileSprite>(
		deviceResources, tileset->GetImageFilename(),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		0, 4);
	*/
	auto playerSprite = make_shared<hosTileTileSprite>(
		tileset, 0,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	//playerSprite->SetScale(2.0f);
	//m_player = std::make_shared<Player>(playerSprite);
	//renderer->AddSprite(playerSprite);

	// TODO: Add actual tile support.

	m_camera = std::make_unique<Camera>(renderer, m_player);
}

void Game::Update(const DX::StepTimer& timer)
{
	m_player->Update(timer);
	m_camera->Update();
}
