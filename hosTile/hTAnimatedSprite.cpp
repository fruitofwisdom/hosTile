#include "pch.h"
#include "hTAnimatedSprite.h"

#include <fstream>
#include "hTException.h"
#include "hTRenderer.h"
#include "Other\json.hpp"

using namespace DirectX;
using namespace hosTile;
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
	// TODO: Parse animation file to build frame data.
	ifstream animationFile(animationFilename);
	if (animationFile.is_open())
	{
		try
		{
			nlohmann::json animationJson;
			animationFile >> animationJson;

			string spriteFilename = animationJson["frames"][0]["filename"];
			// sprite sheets are originally .ase files, but converted to .dds
			spriteFilename.replace(spriteFilename.find(".ase"), string(".ase").length(), ".dds");
			SetSprite(deviceResources, spriteFilename);

			m_numFrames = (unsigned int)animationJson["frames"].size();
			m_speed = animationJson["frames"][0]["duration"];

			// update the actual size of the sprite based on the number of frames
			m_width /= m_numFrames;

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
