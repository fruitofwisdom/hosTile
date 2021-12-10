#pragma once

#include <string>
#include "Other/DeviceResources.h"

namespace hosTile
{
	class hTTileset
	{
	public:
		hTTileset(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string tilesetSource);

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

		unsigned int m_imageWidth;
		unsigned int m_imageHeight;
		unsigned int m_tileCount;
		unsigned int m_tileWidth;
		unsigned int m_tileHeight;
	};
}
