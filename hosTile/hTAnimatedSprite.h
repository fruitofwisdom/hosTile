#pragma once

#include "hTFrameData.h"
#include "hTSimpleSprite.h"
#include <vector>

// An hTAnimatedSprite is a sprite sheet that plays an animation by moving the UVs across the
// the frames of an animation.
namespace hosTile
{
	class hTAnimatedSprite : public hTSimpleSprite
	{
	public:
		hTAnimatedSprite(
			const DX::DeviceResources* deviceResources, std::string animationFilename,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Update(float elapsedSeconds);

		// Replace the sprite with a new one.
		void PlayAnimation(const DX::DeviceResources* deviceResources, std::string animationFilename);

		// TODO: Get collision box, hit box, hurt box, etc.

	private:
		// Update the vertices' data after the animation has changed.
		void UpdateVertices();

		unsigned int m_currentFrame;
		unsigned int m_numFrames;

		float m_currentTime;		// in seconds
		unsigned int m_speed;		// in milliseconds
	};
}
