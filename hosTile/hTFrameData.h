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
	};

	// A frame of data with possible collision, hit box, and hurt box.
	struct hTFrameData
	{
		hTRegion m_collision;
		hTRegion m_hitBox;
		hTRegion m_hurtBox;
	};
}
