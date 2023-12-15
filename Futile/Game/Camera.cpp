#include "pch.h"
#include "Camera.h"

using namespace Futile;

Camera::Camera(hosTile::hTRenderer& renderer, const GameObject* focus)
:	m_renderer(&renderer),
	m_focus(focus)
{
	;
}

void Camera::Update()
{
	DirectX::XMFLOAT3 cameraPosition = m_renderer->GetCameraPosition();
	if (m_focus != nullptr)
	{
		cameraPosition.x = m_focus->GetPosition().x * m_renderer->GetScale();
		cameraPosition.y = m_focus->GetPosition().y * m_renderer->GetScale();
		// Leave the camera's z in the same place.
	}
	m_renderer->SetCameraPosition(cameraPosition);
}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return m_focus->GetPosition();
}
