#include "pch.h"
#include "Game.h"

#include <fstream>
#include "hosTile\hosTileSprite.h"
#include "Other\json.hpp"

using namespace hosTileSample;
using namespace std;

Game::Game(std::shared_ptr<hosTile::hosTileRenderer> renderer)
{
	ifstream mapFile("futile_map.json");
	nlohmann::json mapJson;
	mapFile >> mapJson;
	string tilesetSource = mapJson["tilesets"][0]["source"];
	// the tileset is kept internally as a .tsx file, but exported as a .json
	tilesetSource.replace(tilesetSource.find(".tsx"), string(".tsx").length(), ".json");

	ifstream tilesetFile(tilesetSource);
	nlohmann::json tilesetJson;
	tilesetFile >> tilesetJson;
	string tilesetImage = tilesetJson["image"];
	// the image is kept internally as a .png file, but exported as a .dds
	tilesetImage.replace(tilesetImage.find(".png"), string(".png").length(), ".dds");

	// TODO: Create a proper map class.
	auto map = renderer->CreateSprite(tilesetImage);
	map->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	map->SetScale(2.0f);
	renderer->AddSprite(map);

	// TODO: Handle tiles in a simple way instead of "sub-sprites"?
	auto playerSprite = renderer->CreateSprite(tilesetImage, 0, 4);
	playerSprite->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f));
	playerSprite->SetScale(2.0f);
	m_player = std::make_shared<Player>(playerSprite);
	renderer->AddSprite(playerSprite);

	// TODO: Add actual tile support.

	m_camera = std::make_unique<Camera>(renderer, m_player);
}

void Game::Update(const DX::StepTimer& timer)
{
	m_player->Update(timer);
	m_camera->Update();
}
