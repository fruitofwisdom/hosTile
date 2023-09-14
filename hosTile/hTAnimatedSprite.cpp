#include "pch.h"
#include "hTAnimatedSprite.h"

#include <fstream>
#include "hTException.h"
#include "hTRenderer.h"
#include "Other\json.hpp"

using namespace DirectX;
using namespace hosTile;
using namespace nlohmann;
using namespace std;

hTAnimatedSprite::hTAnimatedSprite(
	const DX::DeviceResources* deviceResources, string animationFilename,
	XMFLOAT3 position)
:	hTSimpleSprite(deviceResources, "", position),
	m_currentFrame(0),
	m_currentTime(0.0f),
	m_animationDone(false),
	m_looping(true),
	m_numFrames(0),
	m_speed(100)
{
	PlayAnimation(deviceResources, animationFilename);
}

void hTAnimatedSprite::Update(float elapsedSeconds)
{
	// Progress m_currentTime and loop back when we've passed the end, if looping.
	m_currentTime += elapsedSeconds;
	float length = m_numFrames * m_speed / 1000.0f;
	if (m_currentTime > length)
	{
		if (m_looping)
		{
			m_currentTime -= length;
		}
		else
		{
			m_currentTime = length;
			m_animationDone = true;
		}
	}
	m_currentFrame = (int)(m_currentTime / length * m_numFrames);
}

void hTAnimatedSprite::PlayAnimation(const DX::DeviceResources* deviceResources, string animationFilename, bool looping)
{
	if (m_currentAnimation == animationFilename)
	{
		return;
	}

	ifstream animationFile(animationFilename);
	if (animationFile.is_open())
	{
		try
		{
			json animationJson;
			animationFile >> animationJson;

			string spriteFilename = animationJson["frames"][0]["filename"];
			// sprite sheets are originally .aseprite or .ase files, but converted to .dds
			if (spriteFilename.find(".aseprite") != string::npos)
			{
				spriteFilename.replace(spriteFilename.find(".aseprite"), string(".aseprite").length(), ".dds");
			}
			else if(spriteFilename.find(".ase") != string::npos)
			{
				spriteFilename.replace(spriteFilename.find(".ase"), string(".ase").length(), ".dds");
			}
			SetSprite(deviceResources, spriteFilename);

			// Reset the animation.
			m_currentAnimation = animationFilename;
			m_currentFrame = 0;
			m_currentTime = 0.0f;
			m_animationDone = false;
			m_looping = looping;
			m_numFrames = (unsigned int)animationJson["frames"].size();
			m_speed = animationJson["frames"][0]["duration"];
			m_frameData.clear();

			// update the actual size of the sprite based on the number of frames
			m_width /= m_numFrames;

			for (unsigned int i = 0; i < m_numFrames; ++i)
			{
				hTFrameData frameData;
				m_frameData.push_back(frameData);
			}
			// The program Aseprite has a concept of slices that we can use to define regions for
			// collision, hit boxes, and hurt boxes. Load these into per-frame data.
			for (json slice : animationJson["meta"]["slices"])
			{
				if (slice["name"] == "Collision")
				{
					for (json key : slice["keys"])
					{
						m_frameData[key["frame"]].m_collision.hasRegion = true;
						m_frameData[key["frame"]].m_collision.x = key["bounds"]["x"];
						m_frameData[key["frame"]].m_collision.y = key["bounds"]["y"];
						m_frameData[key["frame"]].m_collision.width = key["bounds"]["w"];
						m_frameData[key["frame"]].m_collision.height = key["bounds"]["h"];
					}
				}
				if (slice["name"] == "Hit Box")
				{
					for (json key : slice["keys"])
					{
						m_frameData[key["frame"]].m_hitBox.hasRegion = true;
						m_frameData[key["frame"]].m_hitBox.x = key["bounds"]["x"];
						m_frameData[key["frame"]].m_hitBox.y = key["bounds"]["y"];
						m_frameData[key["frame"]].m_hitBox.width = key["bounds"]["w"];
						m_frameData[key["frame"]].m_hitBox.height = key["bounds"]["h"];
					}
				}
				if (slice["name"] == "Hurt Box")
				{
					for (json key : slice["keys"])
					{
						m_frameData[key["frame"]].m_hurtBox.hasRegion = true;
						m_frameData[key["frame"]].m_hurtBox.x = key["bounds"]["x"];
						m_frameData[key["frame"]].m_hurtBox.y = key["bounds"]["y"];
						m_frameData[key["frame"]].m_hurtBox.width = key["bounds"]["w"];
						m_frameData[key["frame"]].m_hurtBox.height = key["bounds"]["h"];
					}
				}
			}

			UpdateVertices();
		}
		catch (hTException& exception)
		{
			// Any hosTile exceptions can be handled generically.
			hTException::HandleException(exception);
		}
	}
}

bool hTAnimatedSprite::AnimationDone() const
{
	return m_animationDone;
}

hTRegion hTAnimatedSprite::GetCollision() const
{
	return m_frameData[m_currentFrame].m_collision;
}

hTRegion hTAnimatedSprite::GetHitBox() const
{
	return m_frameData[m_currentFrame].m_hitBox;
}

hTRegion hTAnimatedSprite::GetHurtBox() const
{
	return m_frameData[m_currentFrame].m_hurtBox;
}

// Update the vertices' data after the position has changed.
void hTAnimatedSprite::UpdateVertices()
{
	// Calculate UV-coordinates.
	float uvAnimatedWidth = 1.0f / m_numFrames;
	float uvXOffset = m_currentFrame * uvAnimatedWidth;
	float uvLeft = uvXOffset;
	float uvRight = uvXOffset + uvAnimatedWidth;
	float uvBottom = 1.0f;
	float uvTop = 0.0f;

	m_vertices[0] =		// 0, bottom-left
	{
		XMFLOAT3(
			(m_width / 2.0f * -1.0f) * m_scale + m_position.x,
			(m_height / 2.0f * -1.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvLeft, uvBottom)
	};
	m_vertices[1] =		// 1, bottom-right
	{
		XMFLOAT3(
			(m_width / 2.0f) * m_scale + m_position.x,
			(m_height / 2.0f * -1.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvRight, uvBottom)
	};
	m_vertices[2] =		// 2, top-right
	{
		XMFLOAT3(
			(m_width / 2.0f) * m_scale + m_position.x,
			(m_height / 2.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvRight, uvTop)
	};
	m_vertices[3] =		// 3, top-left
	{
		XMFLOAT3(
			(m_width / 2.0f * -1.0f) * m_scale + m_position.x,
			(m_height / 2.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvLeft, uvTop)
	};

	// Apply x-flip and y-flip.
	if (m_xFlip)
	{
		SwapUVs(m_vertices[0].tex, m_vertices[1].tex);
		SwapUVs(m_vertices[2].tex, m_vertices[3].tex);
	}
	if (m_yFlip)
	{
		SwapUVs(m_vertices[0].tex, m_vertices[3].tex);
		SwapUVs(m_vertices[1].tex, m_vertices[2].tex);
	}
}
