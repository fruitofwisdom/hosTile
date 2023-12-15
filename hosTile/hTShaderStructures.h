#pragma once

#include <VertexTypes.h>

namespace hosTile
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexPositionTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
	};

	// A simple quad to use with the debug rendering pipeline.
	struct hTQuad
	{
		DirectX::VertexPositionColor mVertices[4];
	};
}
