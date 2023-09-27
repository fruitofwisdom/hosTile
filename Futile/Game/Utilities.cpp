#include "pch.h"
#include "Utilities.h"

using namespace DirectX;
using namespace Futile;
using namespace std;

// Get the angle to a position in degrees from 0.0f to 360.0f.
float Utilities::GetAngleTo(XMVECTOR position)
{
	return XMConvertToDegrees((float)atan2(XMVectorGetY(position), XMVectorGetX(position))) + 180.0f;
}

// Convert a color code like "#FF7700" to a vector like { 1.0f, 0.467f, 0.0f }.
XMVECTORF32 Utilities::HexColorCodeToVector(string hexColorCode)
{
	string r = hexColorCode.substr(1, 2);
	string g = hexColorCode.substr(3, 2);
	string b = hexColorCode.substr(5, 2);

	float x = strtol(r.c_str(), nullptr, 16) / 255.0f;
	float y = strtol(g.c_str(), nullptr, 16) / 255.0f;
	float z = strtol(b.c_str(), nullptr, 16) / 255.0f;

	XMVECTORF32 vector = { x, y, z, 0.0f };
	return vector;
}
