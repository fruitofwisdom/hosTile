#pragma once

#include <string>
#include "DirectX/DeviceResources.h"
#include "hosTileShaderStructures.h"

namespace hosTile
{
	class hosTileSprite
	{
	public:
		hosTileSprite(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::wstring spriteFilename, DirectX::XMFLOAT3 position);
		~hosTileSprite();

		void Update();

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		void UpdateVertices();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		ID3D11ShaderResourceView* m_texture;
		DirectX::XMFLOAT2 m_dimensions;

		DirectX::XMFLOAT3 m_position;
		VertexPositionTex m_vertices[4];
		ID3D11Buffer* m_vertexBuffer;
	};
}
