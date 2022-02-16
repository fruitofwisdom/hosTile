#include "pch.h"
#include "hTFont.h"

#include <codecvt>
#include "DDSTextureLoader.h"
#include "Other/DirectXHelper.h"

using namespace hosTile;
using namespace std;

hTFont::hTFont(
	const DX::DeviceResources* deviceResources,
	string fontFilename, const wchar_t* fontDescription)
:	m_fontFilename(fontFilename),
	m_letterWidth(0),
	m_letterHeight(0),
	m_fontWidth(0),
	m_fontHeight(0)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wideFontFilename = converter.from_bytes(fontFilename);
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), wideFontFilename.c_str(), nullptr, &m_texture)
	);

	// Store the width and height from the texture description.
	ID3D11Resource* resource = nullptr;
	m_texture->GetResource(&resource);
	ID3D11Texture2D* texture2D = nullptr;
	DX::ThrowIfFailed(resource->QueryInterface(&texture2D));
	D3D11_TEXTURE2D_DESC textureDescription;
	texture2D->GetDesc(&textureDescription);
	m_imageWidth = textureDescription.Width;
	m_imageHeight = textureDescription.Height;
	texture2D->Release();
	resource->Release();

	// Build the 2-dimensional array of characters that index the font.
	vector<wchar_t> row;
	unsigned int currentRowWidth = 0;
	for (unsigned int i = 0; i < wcslen(fontDescription); ++i)
	{
		if (fontDescription[i] == '\n')
		{
			m_fontDescription.push_back(row);
			row.clear();
			m_fontHeight++;
			currentRowWidth = 0;
		}
		else
		{
			row.push_back(fontDescription[i]);
			currentRowWidth++;
			if (currentRowWidth > m_fontWidth)
			{
				m_fontWidth = currentRowWidth;
			}
		}
	}
	if (!row.empty())
	{
		m_fontDescription.push_back(row);
		m_fontHeight++;
	}

	// From the number of characters provided and the image size, we can determine the tile size.
	if (m_fontWidth > 0 && m_fontHeight > 0)
	{
		m_letterWidth = m_imageWidth / m_fontWidth;
		m_letterHeight = m_imageHeight / m_fontHeight;
	}
}

ID3D11ShaderResourceView* hTFont::GetTexture() const
{
	return m_texture.Get();
}

unsigned int hTFont::GetImageWidth() const
{
	return m_imageWidth;
}

unsigned int hTFont::GetImageHeight() const
{
	return m_imageHeight;
}

unsigned int hTFont::GetLetterWidth() const
{
	return m_letterWidth;
}

unsigned int hTFont::GetLetterHeight() const
{
	return m_letterHeight;
}

// Returns the offset into the tileset for the provided tile in pixels.
unsigned int hTFont::GetLetterXOffset(wchar_t letter) const
{
	unsigned int toReturn = 0;

	for (int row = 0; row < m_fontDescription.size(); ++row)
	{
		for (int column = 0; column < m_fontDescription[row].size(); ++column)
		{
			if (m_fontDescription[row][column] == letter)
			{
				toReturn = column * m_letterWidth;
			}
		}
	}

	return toReturn;
}

unsigned int hTFont::GetLetterYOffset(wchar_t letter) const
{
	unsigned int toReturn = 0;

	for (int row = 0; row < m_fontDescription.size(); ++row)
	{
		for (int column = 0; column < m_fontDescription[row].size(); ++column)
		{
			if (m_fontDescription[row][column] == letter)
			{
				toReturn = row * m_letterHeight;
			}
		}
	}

	return toReturn;
}
