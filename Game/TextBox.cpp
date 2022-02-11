#include "pch.h"
#include "TextBox.h"

#include <fstream>
#include "hosTile/Other/json.hpp"

using namespace hosTileSample;
using namespace std;

TextBox::TextBox(const hosTile::hTTileset& tileset, string mapFilename)
{
	ifstream textBoxMapFile(mapFilename);
	nlohmann::json textBoxMapJson;
	textBoxMapFile >> textBoxMapJson;
	m_textBoxMap = make_unique<hosTile::hTMap>(&tileset, textBoxMapJson);
}

void TextBox::Update()
{
	m_textBoxMap->Update();
}

hosTile::hTSprite* TextBox::GetSprite() const
{
	return m_textBoxMap.get();
}

void TextBox::SetPosition(DirectX::XMFLOAT3 position)
{
	m_textBoxMap->SetPosition(position);
}

unsigned int TextBox::GetWidth() const
{
	return m_textBoxMap->GetWidth();
}

unsigned int TextBox::GetHeight() const
{
	return m_textBoxMap->GetHeight();
}

void TextBox::SetScale(float scale)
{
	m_textBoxMap->SetScale(scale);
}
