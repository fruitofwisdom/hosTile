#pragma once

#include <vector>
#include "DirectX/DeviceResources.h"
#include "hosTileShaderStructures.h"
#include "hosTileSprite.h"

namespace hosTile
{
	// This sample renderer instantiates a basic rendering pipeline.
	class hosTileRenderer
	{
	public:
		hosTileRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		void Update();
		void Render();

		std::shared_ptr<hosTile::hosTileSprite> CreateSprite(std::wstring spriteFilename) const;
		void AddSprite(std::shared_ptr<hosTile::hosTileSprite> sprite);

	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for the geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

		// Actual buffer data.
		VertexPositionTex* m_vertexBufferData;
		unsigned short* m_indexBufferData;
		ModelViewProjectionConstantBuffer m_constantBufferData;

		// Variables used with the rendering loop.
		bool m_loadingComplete;

		std::vector<std::shared_ptr<hosTile::hosTileSprite>> m_sprites;
	};
}
