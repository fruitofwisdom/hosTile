#pragma once

#include <string>
#include "DirectX/DeviceResources.h"

namespace hosTile
{
	class hosTileSprite
	{
	public:
		hosTileSprite(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string spriteFilename, DirectX::XMFLOAT3 position);
		~hosTileSprite();

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		ID3D11Buffer* m_vertexBuffer;
		ID3D11ShaderResourceView* m_texture;
		DirectX::XMFLOAT3 m_position;
	};
}
