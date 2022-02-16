#include "pch.h"
#include "TextBox.h"

#include <fstream>
#include "hosTile/Other/json.hpp"

using namespace hosTileSample;
using namespace std;

TextBox::TextBox(
	const hosTile::hTTileset* boxTileset, string boxFilename,
	const hosTile::hTFont* font, const wchar_t* text)
{
	ifstream textBoxMapFile(boxFilename);
	nlohmann::json textBoxMapJson;
	textBoxMapFile >> textBoxMapJson;
	m_box = make_unique<hosTile::hTMap>(boxTileset, textBoxMapJson);

	m_text = make_unique<hosTile::hTTextBox>(font, text);
}

void TextBox::Render(hosTile::hTRenderer& renderer)
{
	m_box->Render(renderer);
	// TODO: Add bounds.
	m_text->Render(renderer, m_box->GetPosition());
}

void TextBox::SetPosition(DirectX::XMFLOAT3 position)
{
	m_box->SetPosition(position);
}

unsigned int TextBox::GetWidth() const
{
	return m_box->GetWidth();
}

unsigned int TextBox::GetHeight() const
{
	return m_box->GetHeight();
}

void TextBox::SetScale(float scale)
{
	m_box->SetScale(scale);
	m_text->SetScale(scale);
}
