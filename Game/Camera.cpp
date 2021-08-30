#include "pch.h"
#include "Camera.h"

using namespace hosTileSample;

Camera::Camera(std::shared_ptr<hosTile::hosTileRenderer> renderer, std::shared_ptr<GameObject> focus)
:	m_renderer(renderer),
	m_focus(focus)
{
	;
}

void Camera::Update()
{
	DirectX::XMFLOAT3 cameraPosition = m_renderer->GetCameraPosition();
	cameraPosition.x = m_focus->GetPosition().x;
	cameraPosition.y = m_focus->GetPosition().y;
	// Leave the camera's z in the same place.
	m_renderer->SetCameraPosition(cameraPosition);
}
