#pragma once

#include <vector>
#include "Other/DeviceResources.h"
#include "hTShaderStructures.h"

namespace hosTile { class hTSprite; }

// hTRenderer is the hosTile graphics engine's main rendering component. It manages the creation
// and rendering of sprites and interfacing with the DirectX hardware. Every frame, any hTSprite-
// derived sprite, map, text box, etc. that wants to render must call AddSprite. When Render is
// called, all sprites are drawn in order (with depth via the painter's algorithm) and the list of
// sprites is then cleared.
namespace hosTile
{
	class hTRenderer
	{
	public:
		hTRenderer(DX::DeviceResources* deviceResources);

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		DX::DeviceResources* GetDeviceResources() const;

		// Add an hTSprite-derived object to the list of sprites to render.
		void AddSprite(const hTSprite* sprite);

		// Render all sprites in order and then clear the sprite list.
		void Render();

		DirectX::XMFLOAT3 GetCameraPosition() const;
		void SetCameraPosition(DirectX::XMFLOAT3 cameraPosition);

		// Convert from screen space (pixels) to world space.
		DirectX::XMFLOAT3 ScreenToWorldPosition(int x, int y) const;

		// Set the background clear color.
		void SetClearColor(DirectX::XMVECTORF32 clearColor);

		// A global scale can be applied.
		float GetScale() const;
		void SetScale(float scale);

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
		float m_scale;
	};
}
