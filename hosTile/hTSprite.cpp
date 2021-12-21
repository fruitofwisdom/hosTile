#include "pch.h"
#include "hTSimpleSprite.h"

using namespace DirectX;
using namespace hosTile;

hTSprite::hTSprite(XMFLOAT3 position)
:	m_position(position),
	m_width(0),
	m_height(0),
	m_scale(1.0f),
	m_xFlip(false),
	m_yFlip(false)
{
	;
}

XMFLOAT3 hTSprite::GetPosition() const
{
	return m_position;
}

void hTSprite::SetPosition(XMFLOAT3 position)
{
	m_position = position;
}

unsigned int hTSprite::GetWidth() const
{
	return m_width;
}

unsigned int hTSprite::GetHeight() const
{
	return m_height;
}

void hTSprite::SetScale(float scale)
{
	m_scale = scale;
}

void hTSprite::SetXFlip(bool xFlip)
{
	m_xFlip = xFlip;
}

void hTSprite::SetYFlip(bool yFlip)
{
	m_yFlip = yFlip;
}

// Swaps the values of two UVs. Handy for applying x-flip, y-flip, etc.
void hTSprite::swapUVs(XMFLOAT2& uv1, XMFLOAT2& uv2) const
{
	XMFLOAT2 temp = uv1;
	uv1 = uv2;
	uv2 = temp;
}
