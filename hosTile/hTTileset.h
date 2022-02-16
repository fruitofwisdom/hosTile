#pragma once

#include <string>
#include "Other/DeviceResources.h"

// An hTTileset is a representation of an image that has been broken down to individual tiles that
// can be referenced by tile number. Information describing a tileset is provided by a Tiled-
// formatted .json file.
namespace hosTile
{
	class hTTileset
	{
	public:
		hTTileset(const DX::DeviceResources* deviceResources, std::string tilesetSource);

		const std::string GetImageFilename() const;
		ID3D11ShaderResourceView* GetTexture() const;

		unsigned int GetImageWidth() const;
		unsigned int GetImageHeight() const;
		unsigned int GetTileCount() const;
		unsigned int GetTileWidth() const;
		unsigned int GetTileHeight() const;

		// Returns the offset into the tileset for the provided tile in pixels.
		unsigned int GetTileXOffset(unsigned int tileNum) const;
		unsigned int GetTileYOffset(unsigned int tileNum) const;

	private:
		std::string m_imageFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

		unsigned int m_imageWidth;		// in pixels
		unsigned int m_imageHeight;
		unsigned int m_tileCount;
		unsigned int m_tileWidth;		// in pixels
		unsigned int m_tileHeight;
	};
}
