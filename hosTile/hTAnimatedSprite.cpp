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
	m_numFrames(0),
	m_currentTime(0.0f),
	m_speed(100)
{
	PlayAnimation(deviceResources, animationFilename);
}

void hTAnimatedSprite::Update(float elapsedSeconds)
{
	m_currentTime += elapsedSeconds;
	int currentFrame = m_currentTime * 1000.0f / m_speed;
	currentFrame %= m_numFrames;
	if (currentFrame != m_currentFrame)
	{
		m_currentFrame = currentFrame;
	}
}

void hTAnimatedSprite::PlayAnimation(const DX::DeviceResources* deviceResources, string animationFilename)
{
	ifstream animationFile(animationFilename);
	if (animationFile.is_open())
	{
		try
		{
			json animationJson;
			animationFile >> animationJson;

			string spriteFilename = animationJson["frames"][0]["filename"];
			// sprite sheets are originally .ase files, but converted to .dds
			spriteFilename.replace(spriteFilename.find(".ase"), string(".ase").length(), ".dds");
			SetSprite(deviceResources, spriteFilename);

			m_numFrames = (unsigned int)animationJson["frames"].size();
			m_speed = animationJson["frames"][0]["duration"];

			// update the actual size of the sprite based on the number of frames
			m_width /= m_numFrames;

			m_frameData.clear();
			for (int i = 0; i < m_numFrames; ++i)
			{
				hTFrameData frameData;
				m_frameData.push_back(frameData);
			}
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
