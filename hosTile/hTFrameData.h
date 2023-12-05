#pragma once

#include "hTShaderStructures.h"

namespace hosTile
{
	// Represents a region or rectangle. Note that position is the top-left of the sprite with a
	// positive y-direction being downward. This is the orientation from Aseprite.
	struct hTRegion
	{
		bool hasRegion = false;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;

		bool Intersects(hTRegion other) const
		{
			bool doesIntersect = false;

			// Invalid regions can never intersect.
			if (width == 0 || height == 0 ||
				other.width == 0 || other.height == 0)
			{
				return doesIntersect;
			}

			if (x <= other.x + other.width &&
				x + width >= other.x &&
				y - height <= other.y &&
				y >= other.y - other.height)
			{
				doesIntersect = true;
			}

			return doesIntersect;
		}

		// Return this region as an hTQuad for debug rendering.
		hTQuad AsQuad(DirectX::XMVECTORF32 color) const
		{
			hTQuad quad;
			quad.mVertices[0].position.x = (float)x;
			quad.mVertices[0].position.y = (float)y;
			quad.mVertices[0].position.z = 0.0f;
			DirectX::XMStoreFloat4(&quad.mVertices[0].color, color);
			quad.mVertices[1].position.x = (float)x;
			quad.mVertices[1].position.y = (float)y - height;
			quad.mVertices[1].position.z = 0.0f;
			DirectX::XMStoreFloat4(&quad.mVertices[1].color, color);
			quad.mVertices[2].position.x = (float)x + width;
			quad.mVertices[2].position.y = (float)y - height;
			quad.mVertices[2].position.z = 0.0f;
			DirectX::XMStoreFloat4(&quad.mVertices[2].color, color);
			quad.mVertices[3].position.x = (float)x + width;
			quad.mVertices[3].position.y = (float)y;
			quad.mVertices[3].position.z = 0.0f;
			DirectX::XMStoreFloat4(&quad.mVertices[3].color, color);
			return quad;
		}
	};

	// A frame of data with possible collision, hit box, and hurt box.
	struct hTFrameData
	{
		hTRegion m_collision;
		hTRegion m_hitBox;
		hTRegion m_hurtBox;
	};
}
