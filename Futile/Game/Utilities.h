#pragma once

#include <string>

namespace Futile
{
	namespace Utilities
	{
		// Get the angle to a position in degrees from 0.0f to 360.0f.
		float GetAngleTo(DirectX::XMVECTOR position);

		// Convert a color code like "#FF7700" to a vector like { 1.0f, 0.467f, 0.0f }.
		DirectX::XMVECTORF32 HexColorCodeToVector(std::string hexColorCode);
	};
}
