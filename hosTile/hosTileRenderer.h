﻿#pragma once

#include <vector>
#include "DirectX/DeviceResources.h"
#include "hosTileShaderStructures.h"
#include "hosTileSprite.h"

namespace hosTile
{
	// The hosTile 2D graphics engine main renderer component. Manages the creation and rendering
	// of sprites and interfacing with the DirectX hardware.
	class hosTileRenderer
	{
	public:
		hosTileRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		void Update();
		void Render();

		// CreateSprite will return a new hosTileSprite that game classes can use and manage.
		// These sprites will then need to be added to the renderer with AddSprite.
		std::shared_ptr<hosTileSprite> CreateSprite(std::wstring spriteFilename) const;
		void AddSprite(std::shared_ptr<hosTileSprite> sprite);

	private:
		// Copy each sprite's vertices into the vertex buffer.
		void FillVertexBuffer();

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

		bool m_loadingComplete;

		// TODO: Can this be a not-fixed number?
		static const int MaxSprites = 1024;
		std::vector<std::shared_ptr<hosTileSprite>> m_sprites;
	};
}
