#include "pch.h"
#include "Game.h"

#include "hosTile\hosTileSprite.h"

using namespace hosTileSample;

Game::Game(const std::shared_ptr<hosTile::hosTileRenderer> renderer)
:	m_renderer(renderer)
{
	// TODO: Create a proper map class?
	auto map = m_renderer->CreateSprite(L"Assets/NES - Final Fantasy - Castle Corneria.dds");
	map->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_renderer->AddSprite(map);

	// TODO: Create a proper player class.
	auto player = m_renderer->CreateSprite(L"Assets/NES - Final Fantasy - Warrior.dds");
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f));
	m_renderer->AddSprite(player);
}

Game::~Game()
{
	;
}

void Game::Update(DX::StepTimer const& timer)
{
	;
}
