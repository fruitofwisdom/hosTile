#include "pch.h"
#include "hosTileSprite.h"

#include "DDSTextureLoader11.h"

using namespace hosTile;

hosTileSprite::hosTileSprite(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string spriteFilename, DirectX::XMFLOAT3 position)
:	m_position(position)
{
	HRESULT result = DirectX::CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), L"Assets/NES - Final Fantasy - Castle Corneria.dds", nullptr, &m_texture);
}

ID3D11ShaderResourceView* hosTileSprite::GetTexture() const
{
	return m_texture;
}
