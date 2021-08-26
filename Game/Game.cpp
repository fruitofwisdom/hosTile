#include "pch.h"
#include "Game.h"

#include "hosTile\hosTileSprite.h"

using namespace hosTileSample;

Game::Game(std::shared_ptr<hosTile::hosTileRenderer> renderer)
{
	// TODO: Create a proper map class?
	auto map = renderer->CreateSprite(L"Assets/NES - Final Fantasy - Castle Corneria.dds");
	map->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	renderer->AddSprite(map);

	auto playerSprite = renderer->CreateSprite(L"Assets/NES - Final Fantasy - Warrior.dds", 0, 4);
	playerSprite->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f));
	m_player = std::make_unique<Player>(playerSprite);
	renderer->AddSprite(playerSprite);
}

void Game::Update(const DX::StepTimer& timer)
{
	m_player->Update(timer);
}
