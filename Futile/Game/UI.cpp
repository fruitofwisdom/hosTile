#include "pch.h"
#include "UI.h"

#include "Game.h"
#include "..\hosTile\hTFont.h"

using namespace hosTile;
using namespace Futile;
using namespace std;

UI::UI(string fontFilename, const wchar_t* version)
{
	// TODO: This should be a part of a font JSON file.
	const wchar_t* fontDescription = L"abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890.:,;\'\"(!?)+-*/= ";
	m_font = make_unique<hTFont>(Game::Get().GetRenderer().GetDeviceResources(), fontFilename, fontDescription);

	m_debug = make_unique<hTTextBox>(*m_font, L"");
	m_version = make_unique<hTTextBox>(*m_font, version);
}

void UI::Update()
{
	hTRenderer& renderer = Game::Get().GetRenderer();
	int screenBottom = (int)renderer.GetDeviceResources()->GetLogicalSize().Height
		- (int)(m_debug->GetFont()->GetLetterHeight() * m_debug->GetScale());
	DirectX::XMFLOAT3 debugPosition = renderer.ScreenToWorldPosition(
		(int)renderer.GetDeviceResources()->GetLogicalSize().Width
		- (int)(m_debug->GetFont()->GetLetterWidth() * (unsigned int)wcslen(m_debug->GetText()) * m_debug->GetScale()),
		screenBottom - (int)(m_debug->GetFont()->GetLetterHeight() * m_debug->GetScale()));
	DirectX::XMFLOAT3 versionPosition = renderer.ScreenToWorldPosition(
		(int)renderer.GetDeviceResources()->GetLogicalSize().Width
		- (int)(m_version->GetFont()->GetLetterWidth() * (unsigned int)wcslen(m_version->GetText()) * m_version->GetScale()),
		screenBottom);
	m_debug->SetPosition(debugPosition);
	m_version->SetPosition(versionPosition);
}

void UI::Render()
{
	m_debug->Render(Game::Get().GetRenderer());
	m_version->Render(Game::Get().GetRenderer());
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
