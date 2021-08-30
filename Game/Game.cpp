#include "pch.h"
#include "Game.h"

#include "hosTile\hosTileSprite.h"

using namespace hosTileSample;

Game::Game(std::shared_ptr<hosTile::hosTileRenderer> renderer)
{
	// TODO: Create a proper map class?
	auto map = renderer->CreateSprite(L"Assets/map.dds");
	map->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	map->SetScale(2.0f);
	renderer->AddSprite(map);

	auto playerSprite = renderer->CreateSprite(L"Assets/player.dds", 0, 4);
	playerSprite->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f));
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
