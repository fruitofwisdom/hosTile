#pragma once

namespace hosTile
{
	// Represents a region or rectangle.
	struct hTRegion
	{
		bool hasRegion = false;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;

		bool Intersects(hTRegion other)
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
				y <= other.y + other.height &&
				y + height >= other.y)
			{
				doesIntersect = true;
			}

			return doesIntersect;
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
