#include "pch.h"
#include "Game.h"

#include "hosTile\hosTileSprite.h"

using namespace hosTileSample;

Game::Game(const std::shared_ptr<hosTile::Sample3DSceneRenderer> sceneRenderer)
:	m_sceneRenderer(sceneRenderer)
{
	// TODO: Create a proper map class?
	auto map = m_sceneRenderer->CreateSprite(L"Assets/NES - Final Fantasy - Castle Corneria.dds");
	map->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_sceneRenderer->AddSprite(map);

	// TODO: Create a proper player class.
	auto player = m_sceneRenderer->CreateSprite(L"Assets/NES - Final Fantasy - Warrior.dds");
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f));
	m_sceneRenderer->AddSprite(player);
}

Game::~Game()
{
	;
}

void Game::Update(DX::StepTimer const& timer)
{
	;
}
