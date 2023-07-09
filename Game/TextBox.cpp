#include "pch.h"
#include "TextBox.h"

#include <fstream>
#include "hosTile/hTFont.h"
#include "hosTile/Other/json.hpp"

using namespace Futile;
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
	m_text->SetBounds(GetWidth() - font->GetLetterWidth() * 2, font->GetLetterHeight() * 4);
}

void TextBox::Render(hosTile::hTRenderer& renderer)
{
	m_box->Render(renderer);
	m_text->Render(renderer);
}

void TextBox::SetPosition(DirectX::XMFLOAT3 position)
{
	m_box->SetPosition(position);

	// The text starts one tile inwards and two tiles up from the middle.
	DirectX::XMFLOAT3 topLeft =
	{
		m_box->GetPosition().x - GetWidth() / 2.0f + (float)m_text->GetFont()->GetLetterWidth() * m_box->GetScale(),
		m_box->GetPosition().y + (float)(m_text->GetFont()->GetLetterHeight() * 2) * m_box->GetScale(),
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

void TextBox::SetScale(float scale)
{
	m_box->SetScale(scale);
	m_text->SetScale(scale);
}
