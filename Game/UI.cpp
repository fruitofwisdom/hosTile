#include "pch.h"
#include "UI.h"

#include "hosTile/hTFont.h"

using namespace hosTile;
using namespace hosTileSample;
using namespace std;

UI::UI(hTRenderer& renderer, const hTFont* font)
:	m_renderer(&renderer)
{
	m_debug = make_unique<hTTextBox>(font, L"");
	m_version = make_unique<hTTextBox>(font, L"hosTile v0.4.0-alpha");
}

void UI::Update()
{
	int screenBottom = (int)m_renderer->GetDeviceResources()->GetLogicalSize().Height
		- (int)(m_debug->GetFont()->GetLetterHeight() * m_debug->GetScale());
	DirectX::XMFLOAT3 debugPosition = m_renderer->ScreenToWorldPosition(
		(int)m_renderer->GetDeviceResources()->GetLogicalSize().Width
		- (int)(m_debug->GetFont()->GetLetterWidth() * (unsigned int)wcslen(m_debug->GetText()) * m_debug->GetScale()),
		screenBottom - m_debug->GetFont()->GetLetterHeight() * m_debug->GetScale());
	DirectX::XMFLOAT3 versionPosition = m_renderer->ScreenToWorldPosition(
		(int)m_renderer->GetDeviceResources()->GetLogicalSize().Width
		- (int)(m_version->GetFont()->GetLetterWidth() * (unsigned int)wcslen(m_version->GetText()) * m_version->GetScale()),
		screenBottom);
	m_debug->SetPosition(debugPosition);
	m_version->SetPosition(versionPosition);
}

void UI::Render()
{
	m_debug->Render(*m_renderer);
	m_version->Render(*m_renderer);
}

void UI::SetScale(float scale)
{
	m_debug->SetScale(scale);
	m_version->SetScale(scale);
}

void UI::SetDebugText(const wchar_t* debugText)
{
	m_debug->SetText(debugText);
}
