#include "pch.h"
#include "hosTileTileset.h"

#include <codecvt>
#include <fstream>
#include <locale>
#include "DDSTextureLoader.h"
#include "Other/DirectXHelper.h"
#include "Other/json.hpp"

using namespace hosTile;
using namespace std;

hosTileTileset::hosTileTileset(
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
}

const string hosTileTileset::GetImageFilename() const
{
	return m_imageFilename;
}

ID3D11ShaderResourceView* hosTileTileset::GetTexture() const
{
	return m_texture.Get();
}
