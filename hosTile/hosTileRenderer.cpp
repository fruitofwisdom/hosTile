#include "pch.h"
#include "hosTileRenderer.h"

#include "DirectX/DDSTextureLoader11.h"
#include "DirectX/DirectXHelper.h"

using namespace DirectX;
using namespace hosTile;
using namespace std;
using namespace Windows::Foundation;

hosTileRenderer::hosTileRenderer(const shared_ptr<DX::DeviceResources>& deviceResources)
:	m_deviceResources(deviceResources),
	m_loadingComplete(false)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Load the shaders and create all resources and data that rely on the DirectX device.
void hosTileRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"hosTileVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"hosTilePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const vector<byte>& fileData)
	{
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		// Create the input-layout object.
		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
				)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const vector<byte>& fileData)
	{
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});

	// Once both shaders are loaded, create the input buffers, sampler state, and blend state.
	auto finishSetupTask = (createPSTask && createVSTask).then([this]()
	{
		// Pre-allocate room for MaxSprites-worth of vertex data and index data.
		m_vertexBufferData = new VertexPositionTex[MaxSprites * 4];
		m_indexBufferData = new unsigned short[MaxSprites * 6];
		for (int i = 0; i < MaxSprites; ++i)
		{
			ZeroMemory(&m_vertexBufferData[i], sizeof(VertexPositionTex) * 4);
			// Load mesh indices. Each trio of indices represents a triangle to be rendered on the
			// screen. Note the clockwise winding order.
			m_indexBufferData[i * 6] = 0 + i * 4;
			m_indexBufferData[i * 6 + 1] = 2 + i * 4;
			m_indexBufferData[i * 6 + 2] = 1 + i * 4;
			m_indexBufferData[i * 6 + 3] = 0 + i * 4;
			m_indexBufferData[i * 6 + 4] = 3 + i * 4;
			m_indexBufferData[i * 6 + 5] = 2 + i * 4;
		}

		// Create the vertex buffer.
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = m_vertexBufferData;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionTex) * MaxSprites * 4, D3D11_BIND_VERTEX_BUFFER);
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);

		// Create the index buffer.
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = m_indexBufferData;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * MaxSprites * 6, D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateSamplerState(
				&samplerDesc,
				&m_samplerState
				)
			);

		D3D11_BLEND_DESC blendState;
		ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
		blendState.RenderTarget[0].BlendEnable = TRUE;
		blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBlendState(&blendState, &m_blendState)
			);
	});

	// Once all tasks have finished, we are ready to render.
	finishSetupTask.then([this]() {
		m_loadingComplete = true;
	});
}

// Initializes view parameters when the window size changes.
void hosTileRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	XMMATRIX orthoMatrix = XMMatrixOrthographicLH(outputSize.Width, outputSize.Height, 0.01f, 1000.0f);
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(orthoMatrix * orientationMatrix)
		);

	static const XMVECTORF32 eye = { 0.0f, 0.0f, -1.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
}

void hosTileRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
	m_blendState.Reset();
	m_samplerState.Reset();

	delete m_vertexBufferData;
	delete m_indexBufferData;
}

// Called once per frame, calculate the model and view matrices.
void hosTileRenderer::Update()
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));
}

// Renders one frame using the vertex and pixel shaders.
void hosTileRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	// TODO: Upgrade this to ID3D11DeviceContext4.
	ID3D11DeviceContext3* deviceContext = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	deviceContext->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
		);

	// Copy each sprite's vertices into the vertex buffer.
	FillVertexBuffer();

	// Bind the vertex buffer to the IA stage.
	UINT stride = sizeof(VertexPositionTex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	// Bind the index buffer to the IA stage.
	deviceContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	// Set the input layout.
	deviceContext->IASetInputLayout(m_inputLayout.Get());

	// Set the primitive topology.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Attach our vertex shader.
	deviceContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	deviceContext->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
		);

	// Attach our pixel shader.
	deviceContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	deviceContext->PSSetSamplers(
		0,
		1,
		m_samplerState.GetAddressOf()
		);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;
	deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, sampleMask);

	// Draw the sprites.
	UINT numSprites = 0;
	for (auto sprite : m_sprites)
	{
		// Bind each sprite's individual texture.
		ID3D11ShaderResourceView* texture = sprite->GetTexture();
		deviceContext->PSSetShaderResources(
			0,
			1,
			&texture
			);

		deviceContext->DrawIndexed(
			6,
			numSprites * 6,
			0
			);

		numSprites++;
	}
}

shared_ptr<hosTileSprite> hosTileRenderer::CreateSprite(wstring spriteFilename) const
{
	return shared_ptr<hosTileSprite>(new hosTileSprite(m_deviceResources, spriteFilename));
}

void hosTileRenderer::AddSprite(shared_ptr<hosTileSprite> sprite)
{
	m_sprites.push_back(sprite);
}

// Copy each sprite's vertices into the vertex buffer.
void hosTileRenderer::FillVertexBuffer()
{
	// TODO: Upgrade this to ID3D11DeviceContext4.
	ID3D11DeviceContext3* deviceContext = m_deviceResources->GetD3DDeviceContext();

	// Copy each sprite's vertices into the data for the vertex buffer.
	UINT numSprites = 0;
	for (auto sprite : m_sprites)
	{
		const VertexPositionTex* vertexData = sprite->GetVertices();
		memcpy(&m_vertexBufferData[numSprites * 4], vertexData, sizeof(VertexPositionTex) * 4);
		numSprites++;
	}

	// Map the data for the vertex buffer into the actual vertex buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	size_t vertexBufferDataSize = numSprites * sizeof(VertexPositionTex) * 4;
	memcpy(mappedResource.pData, m_vertexBufferData, vertexBufferDataSize);
	deviceContext->Unmap(m_vertexBuffer.Get(), 0);
}
