#include "pch.h"
#include "hTTextBox.h"

#include "hTFont.h"
#include "hTRenderer.h"

using namespace DirectX;
using namespace hosTile;

hTTextBox::hTTextBox(const hTFont* font, const wchar_t* text)
:	m_font(font),
	m_text(text)
{
	for (int i = 0; i < wcslen(text); ++i)
	{
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	}

	UpdateVertices();
}

void hTTextBox::Render(hTRenderer& renderer, DirectX::XMFLOAT3 position)
{
	m_position = position;
	UpdateVertices();
	renderer.AddSprite(this);
}

ID3D11ShaderResourceView* hTTextBox::GetTexture() const
{
	return m_font->GetTexture();
}

unsigned int hTTextBox::GetNumVertices() const
{
	return (unsigned int)m_text.length() * 4;
}

const VertexPositionTex* hTTextBox::GetVertices() const
{
	return &m_vertices[0];
}

void hTTextBox::UpdateVertices()
{
	for (int i = 0; i < m_text.length(); ++i)
	{
		wchar_t letter = m_text[i];

		float letterXPosition = i * m_font->GetLetterWidth();
		float letterYPosition = 0.0f;

		// Calculate UV-coordinates.
		float uvTileWidth = (float)m_font->GetLetterWidth() / m_font->GetImageWidth();
		float uvTileHeight = (float)m_font->GetLetterHeight() / m_font->GetImageHeight();
		float uvXOffset = (float)m_font->GetLetterXOffset(letter) / m_font->GetImageWidth();
		float uvYOffset = (float)m_font->GetLetterYOffset(letter) / m_font->GetImageHeight();
		// Bring in the UV borders an imperceptible amount to prevent seams from showing.
		float uvLeft = uvXOffset;
		float uvRight = uvXOffset + uvTileWidth;
		float uvBottom = uvYOffset + uvTileHeight;
		float uvTop = uvYOffset;

		unsigned int vertexOffset = i * 4;
		m_vertices[vertexOffset] =		// 0, bottom-left
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() / 2.0f * -1.0f + letterXPosition)
				* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(m_font->GetLetterHeight() / 2.0f * -1.0f + letterYPosition)
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvLeft, uvBottom)
		};
		m_vertices[vertexOffset + 1] =		// 1, bottom-right
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() / 2.0f + letterXPosition)
				*(m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(m_font->GetLetterHeight() / 2.0f * -1.0f + letterYPosition)
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvRight, uvBottom)
		};
		m_vertices[vertexOffset + 2] =		// 2, top-right
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() / 2.0f + letterXPosition)
				*(m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(m_font->GetLetterHeight() / 2.0f + letterYPosition)
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvRight, uvTop)
		};
		m_vertices[vertexOffset + 3] =		// 3, top-left
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() / 2.0f * -1.0f + letterXPosition)
				*(m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(m_font->GetLetterHeight() / 2.0f + letterYPosition)
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvLeft, uvTop)
		};
	}
}
