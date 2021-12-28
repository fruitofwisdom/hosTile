#include "pch.h"
#include "hTTileset.h"

#include <codecvt>
#include <fstream>
#include <locale>
#include "DDSTextureLoader.h"
#include "hosTile/hTException.h"
#include "hosTile/Other/DirectXHelper.h"
#include "hosTile/Other/json.hpp"

using namespace hosTile;
using namespace std;

hTTileset::hTTileset(
	const shared_ptr<DX::DeviceResources>& deviceResources, string tilesetSource)
{
	ifstream tilesetFile(tilesetSource);
	nlohmann::json tilesetJson;
	tilesetFile >> tilesetJson;
	m_imageFilename = tilesetJson["image"];
	// the image is kept internally as a .png file, but exported as a .dds
	m_imageFilename.replace(m_imageFilename.find(".png"), string(".png").length(), ".dds");

	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wideTilesetFilename = converter.from_bytes(m_imageFilename);
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), wideTilesetFilename.c_str(), nullptr, &m_texture)
	);

	m_imageWidth = tilesetJson["imagewidth"];
	m_imageHeight = tilesetJson["imageheight"];
	m_tileCount = tilesetJson["tilecount"];
	m_tileWidth = tilesetJson["tilewidth"];
	m_tileHeight = tilesetJson["tileheight"];
}

const string hTTileset::GetImageFilename() const
{
	return m_imageFilename;
}

ID3D11ShaderResourceView* hTTileset::GetTexture() const
{
	return m_texture.Get();
}

unsigned int hTTileset::GetImageWidth() const
{
	return m_imageWidth;
}

unsigned int hTTileset::GetImageHeight() const
{
	return m_imageHeight;
}

unsigned int hTTileset::GetTileCount() const
{
	return m_tileCount;
}

unsigned int hTTileset::GetTileWidth() const
{
	return m_tileWidth;
}

unsigned int hTTileset::GetTileHeight() const
{
	return m_tileHeight;
}

// Returns the offset into the tileset for the provided tile in pixels.
unsigned int hTTileset::GetTileXOffset(unsigned int tileNum) const
{
	if (tileNum >= m_tileCount)
	{
		stringstream message;
		message << "Tile number " << tileNum << " is out of range. Tile count is " << m_tileCount << ".";
		throw hTException(message.str());
	}

	unsigned int tilesWide = m_imageWidth / m_tileWidth;
	return (tileNum % tilesWide) * m_tileWidth;
}

unsigned int hTTileset::GetTileYOffset(unsigned int tileNum) const
{
	if (tileNum >= m_tileCount)
	{
		stringstream message;
		message << "Tile number " << tileNum << " is out of range. Tile count is " << m_tileCount << ".";
		throw hTException(message.str());
	}

	unsigned int tilesWide = m_imageWidth / m_tileWidth;
	return (tileNum / tilesWide) * m_tileHeight;
}
