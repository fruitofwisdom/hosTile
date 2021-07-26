#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "DDSTextureLoader11.h"
#include "DirectXHelper.h"

using namespace hosTileSample;

using namespace DirectX;
using namespace Windows::Foundation;

using namespace std;

// Loads vertex and pixel shaders from files and instantiates the geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_indexCount(0),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
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

// Called once per frame, calculate the model and view matrices.
void Sample3DSceneRenderer::Update()
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionTex struct.
	UINT stride = sizeof(VertexPositionTex);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	context->PSSetSamplers(
		0,
		1,
		&m_sampler
	);

	for (auto sprite : m_sprites)
	{
		// Bind each sprite's individual texture.
		ID3D11ShaderResourceView* texture = sprite->GetTexture();
		context->PSSetShaderResources(
			0,
			1,
			&texture
		);

		// Draw the objects.
		context->DrawIndexed(
			m_indexCount,
			0,
			0
		);
	}
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
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
	auto createPSTask = loadPSTask.then([this](const vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});

	// Once both shaders are loaded, create the mesh.
	auto createMeshTask = (createPSTask && createVSTask).then([this] () {

		// Load mesh vertices. Each vertex has a position and a texture coordinate.
		float width = 544.0f;
		float height = 544.0f;
		static const VertexPositionTex meshVertices[] = 
		{
			{XMFLOAT3(width / 2 * -1, height / 2 * -1, 0.0f), XMFLOAT2(0.0f, 1.0f)},	// 0, bottom-left
			{XMFLOAT3(width / 2, height / 2* -1, 0.0f), XMFLOAT2(1.0f, 1.0f)},	// 1, bottom-right
			{XMFLOAT3(width / 2, height / 2, 0.0f), XMFLOAT2(1.0f, 0.0f)},		// 2, top-right
			{XMFLOAT3(width / 2 * -1, height / 2, 0.0f), XMFLOAT2(0.0f, 0.0f)},	// 3, top-left
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = meshVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(meshVertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen. Note the
		// clockwise winding order.
		static const unsigned short meshIndices [] =
		{
			0,2,1,
			0,3,2
		};

		m_indexCount = ARRAYSIZE(meshIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = meshIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(meshIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		// The sampler does not use anisotropic filtering, so this parameter is ignored.
		samplerDesc.MaxAnisotropy = 0;

		// Specify how texture coordinates outside of the range 0..1 are resolved.
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		// Use no special MIP clamping or bias.
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Don't use a comparison function.
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		// Border address mode is not used, so this parameter is ignored.
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateSamplerState(
				&samplerDesc,
				&m_sampler
			)
		);
	});

	// Once the mesh data is loaded, the object is ready to be rendered.
	createMeshTask.then([this] () {
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

void Sample3DSceneRenderer::AddSprite(shared_ptr<hosTile::hosTileSprite> sprite)
{
	m_sprites.push_back(sprite);
}
