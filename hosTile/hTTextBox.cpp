#include "pch.h"
#include "hTTextBox.h"

#include "hTFont.h"
#include "hTRenderer.h"

using namespace DirectX;
using namespace hosTile;

hTTextBox::hTTextBox(
	const hTFont* font, const wchar_t* text,
	XMFLOAT3 position,
	unsigned int boundingWidth, unsigned int boundingHeight)
:	hTSprite(position),
	m_font(font),
	m_text(text),
	m_boundingWidth(boundingWidth),
	m_boundingHeight(boundingHeight)
{
	SetText(text);
}

void hTTextBox::Render(hTRenderer& renderer)
{
	UpdateVertices();
	renderer.AddSprite(this);
}

ID3D11ShaderResourceView* hTTextBox::GetTexture() const
{
	return m_font->GetTexture();
}

unsigned int hTTextBox::GetNumVertices() const
{
	return m_verticesToRender;
}

const VertexPositionTex* hTTextBox::GetVertices() const
{
	return &m_vertices[0];
}

const hTFont* hTTextBox::GetFont() const
{
	return m_font;
}

const wchar_t* hTTextBox::GetText() const
{
	return m_text.c_str();
}

void hTTextBox::SetText(const wchar_t* text)
{
	m_text = text;

	// Make room for vertices for each letter, though not all may render.
	m_vertices.clear();
	for (int i = 0; i < wcslen(m_text.c_str()); ++i)
	{
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
		m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
	}
	m_verticesToRender = (unsigned int)wcslen(m_text.c_str()) * 4;
	UpdateVertices();
}

void hTTextBox::SetBounds(unsigned int boundingWidth, unsigned int boundingHeight)
{
	m_boundingWidth = boundingWidth;
	m_boundingHeight = boundingHeight;
}

int hTTextBox::DistanceToWhitespace(int from) const
{
	int toReturn = 0;

	for (int i = from; i < m_text.length(); ++i)
	{
		if (m_text[i] == ' ' || m_text[i] == '\t' || m_text[i] == '\n')
		{
			toReturn = i - from;
			break;
		}
	}

	return toReturn;
}

void hTTextBox::UpdateVertices()
{
	int i = 0;
	float letterXPosition = 0.0f;
	float letterYPosition = 0.0f;
	for (; i < m_text.length(); ++i)
	{
		wchar_t letter = m_text[i];

		// \n is a manual line wrap.
		if (letter == '\n')
		{
			letterXPosition = 0.0f;
			letterYPosition -= m_font->GetLetterHeight();
			continue;
		}

		// Handle word wrapping.
		if (m_boundingWidth > 0 && m_boundingHeight > 0)
		{
			if (letterXPosition + DistanceToWhitespace(i) * m_font->GetLetterWidth() > m_boundingWidth)
			{
				letterXPosition = 0.0f;
				letterYPosition -= m_font->GetLetterHeight();
			}
			if (abs(letterYPosition) >= m_boundingHeight)
			{
				break;
			}
		}

		// Calculate UV-coordinates.
		float uvTileWidth = (float)m_font->GetLetterWidth() / m_font->GetImageWidth();
		float uvTileHeight = (float)m_font->GetLetterHeight() / m_font->GetImageHeight();
		float uvXOffset = (float)m_font->GetLetterXOffset(letter) / m_font->GetImageWidth();
		float uvYOffset = (float)m_font->GetLetterYOffset(letter) / m_font->GetImageHeight();
		float uvLeft = uvXOffset;
		float uvRight = uvXOffset + uvTileWidth;
		float uvBottom = uvYOffset + uvTileHeight;
		float uvTop = uvYOffset;

		unsigned int vertexOffset = i * 4;
		m_vertices[vertexOffset] =		// 0, bottom-left
		{
			XMFLOAT3(
				letterXPosition
				* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(letterYPosition - m_font->GetLetterHeight())
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvLeft, uvBottom)
		};
		m_vertices[vertexOffset + 1] =		// 1, bottom-right
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() + letterXPosition)
				* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				(letterYPosition - m_font->GetLetterHeight())
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvRight, uvBottom)
		};
		m_vertices[vertexOffset + 2] =		// 2, top-right
		{
			XMFLOAT3(
				(m_font->GetLetterWidth() + letterXPosition)
				* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				letterYPosition
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvRight, uvTop)
		};
		m_vertices[vertexOffset + 3] =		// 3, top-left
		{
			XMFLOAT3(
				letterXPosition
				* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
				letterYPosition
				* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
				m_position.z
			),
			XMFLOAT2(uvLeft, uvTop)
		};

		letterXPosition += m_font->GetLetterWidth();
	}

	// If not all letters fit, we need to adjust how many vertices to render.
	m_verticesToRender = i * 4;
}
