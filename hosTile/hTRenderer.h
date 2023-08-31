#pragma once

#include <vector>
#include "Other/DeviceResources.h"
#include "hTShaderStructures.h"

namespace hosTile { class hTSprite; }

namespace hosTile
{
	// The hosTile 2D graphics engine main renderer component. Manages the creation and rendering
	// of sprites and interfacing with the DirectX hardware.
	class hTRenderer
	{
	public:
		hTRenderer(DX::DeviceResources* deviceResources);

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		void Render();

		DX::DeviceResources* GetDeviceResources() const;

		// Once a sprite derived from hTSprite has been created, it needs to be added to the
		// renderer with AddSprite. The list of sprites is cleared after each call to Render.
		void AddSprite(const hTSprite* sprite);

		DirectX::XMFLOAT3 GetCameraPosition() const;
		void SetCameraPosition(DirectX::XMFLOAT3 cameraPosition);

		// Convert from screen space (pixels) to world space.
		DirectX::XMFLOAT3 ScreenToWorldPosition(int x, int y) const;

		void SetClearColor(DirectX::XMVECTORF32 clearColor);

	private:
		// Copy each sprite's vertices into the vertex buffer.
		void FillVertexBuffer();

		// Update the constant buffer based on the camera's position, focus, and up vectors.
		void UpdateConstantBuffer();

		DX::DeviceResources* m_deviceResources;

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

		static const int MaxSprites = 10000;
		std::vector<const hTSprite*> m_sprites;

		DirectX::XMVECTORF32 m_cameraPosition;
		DirectX::XMVECTORF32 m_cameraFocus;
		DirectX::XMVECTORF32 m_cameraUp;

		DirectX::XMVECTORF32 m_clearColor;
	};
}
