#pragma once

#include <string>
#include <vector>
#include "Other/DeviceResources.h"

// An hTFont is a representation of a font that has been broken down into individual tiles. The
// layout of the font is provided at creation and then text can be rendered arbitrarily.
namespace hosTile
{
	class hTFont
	{
	public:
		hTFont(
			const DX::DeviceResources* deviceResources,
			std::string fontFilename, const wchar_t* fontDescription);

		ID3D11ShaderResourceView* GetTexture() const;

		unsigned int GetImageWidth() const;
		unsigned int GetImageHeight() const;
		unsigned int GetLetterWidth() const;
		unsigned int GetLetterHeight() const;

		// Returns the offset into the tileset for the provided tile in pixels.
		unsigned int GetLetterXOffset(wchar_t letter) const;
		unsigned int GetLetterYOffset(wchar_t letter) const;

	private:
		std::string m_fontFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

		unsigned int m_imageWidth;		// these are all in pixels
		unsigned int m_imageHeight;
		unsigned int m_letterWidth;
		unsigned int m_letterHeight;

		std::vector<std::vector<wchar_t>> m_fontDescription;
		unsigned int m_fontWidth;		// in letters (total columns)
		unsigned int m_fontHeight;		// in letters (total rows)
	};
}
