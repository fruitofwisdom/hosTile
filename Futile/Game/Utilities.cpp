#include "pch.h"
#include "Utilities.h"

using namespace Futile;
using namespace std;

DirectX::XMVECTORF32 Utilities::HexColorCodeToVector(string hexColorCode)
{
	string r = hexColorCode.substr(1, 2);
	string g = hexColorCode.substr(3, 2);
	string b = hexColorCode.substr(5, 2);

	float x = strtol(r.c_str(), nullptr, 16) / 255.0f;
	float y = strtol(g.c_str(), nullptr, 16) / 255.0f;
	float z = strtol(b.c_str(), nullptr, 16) / 255.0f;

	DirectX::XMVECTORF32 vector = { x, y, z, 0.0f };
	return vector;
}
