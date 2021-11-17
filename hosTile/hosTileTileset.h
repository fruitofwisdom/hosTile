#pragma once

#include <string>
#include "Other/DeviceResources.h"

namespace hosTile
{
	class hosTileTileset
	{
	public:
		hosTileTileset(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string tilesetSource);

		const std::string GetImageFilename() const;
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		std::string m_imageFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	};
}
