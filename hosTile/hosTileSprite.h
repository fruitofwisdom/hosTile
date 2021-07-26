#pragma once

#include <string>
#include "DeviceResources.h"

namespace hosTile
{
	class hosTileSprite
	{
	public:
		hosTileSprite(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string spriteFilename, DirectX::XMFLOAT3 position);

		ID3D11ShaderResourceView* GetTexture() const;

	private:
		ID3D11ShaderResourceView* m_texture;
		DirectX::XMFLOAT3 m_position;
	};
}
