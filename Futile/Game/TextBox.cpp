#include "pch.h"
#include "TextBox.h"

#include <fstream>
#include "Game.h"
#include "..\hosTile\Other\json.hpp"

using namespace Futile;
using namespace hosTile;
using namespace std;

TextBox::TextBox(string tilesetFilename, string boxFilename, string fontFilename, const wchar_t* text)
{
	DX::DeviceResources* deviceResources = Game::Get().GetRenderer().GetDeviceResources();

	m_tileset = make_unique<hTTileset>(deviceResources, tilesetFilename);

	ifstream textBoxMapFile(boxFilename);
	nlohmann::json textBoxMapJson;
	textBoxMapFile >> textBoxMapJson;
	m_box = make_unique<hosTile::hTMap>(*m_tileset, textBoxMapJson);

	// TODO: This should be a part of a font JSON file.
	const wchar_t* fontDescription = L"abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890.:,;\'\"(!?)+-*/= ";
	m_font = make_unique<hTFont>(deviceResources, fontFilename, fontDescription);

	m_text = make_unique<hosTile::hTTextBox>(*m_font, text);
	m_text->SetBounds(GetWidth() - m_font->GetLetterWidth() * 2, m_font->GetLetterHeight() * 4);
}

void TextBox::Render()
{
	m_box->Render(Game::Get().GetRenderer());
	m_text->Render(Game::Get().GetRenderer());
}

void TextBox::SetPosition(DirectX::XMFLOAT3 position)
{
	m_box->SetPosition(position);

	// The text starts one tile inwards and two tiles up from the middle.
	DirectX::XMFLOAT3 topLeft =
	{
		m_box->GetPosition().x - GetWidth() / 2.0f + (float)m_text->GetFont()->GetLetterWidth(),
		m_box->GetPosition().y + (float)(m_text->GetFont()->GetLetterHeight() * 2),
		m_box->GetPosition().z
	};
	m_text->SetPosition(topLeft);
}

unsigned int TextBox::GetWidth() const
{
	return (unsigned int)(m_box->GetWidth() * m_box->GetScale());
}

unsigned int TextBox::GetHeight() const
{
	return (unsigned int)(m_box->GetHeight() * m_box->GetScale());
}

bool TextBox::Contains(float x, float y) const
{
	return m_box->Contains(x, y);
}
