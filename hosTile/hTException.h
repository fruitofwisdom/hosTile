#pragma once

#include <codecvt>
#include <exception>
#include <locale>

namespace hosTile
{
	class hTException : public std::exception
	{
	public:
		hTException(std::string message)
		:	m_message(message)
		{
			;
		}

		const char* what() const throw()
		{
			return m_message.c_str();
		}

		static void HandleException(hTException& exception)
		{
			Windows::UI::Core::CoreWindow::GetForCurrentThread()->Dispatcher->RunAsync(
				Windows::UI::Core::CoreDispatcherPriority::Normal,
				ref new Windows::UI::Core::DispatchedHandler([exception]()
					{
						// Convert the exception message from const char* to Platform::String^.
						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						std::wstring wideExceptionMessage = converter.from_bytes(exception.what());
						Platform::String^ exceptionMessage = ref new Platform::String(wideExceptionMessage.c_str());
						// Then display it in the simplest possible dialog box.
						Windows::UI::Popups::MessageDialog popup(exceptionMessage, L"An exception occurred!");
						popup.ShowAsync();
					}));
		}

	private:
		std::string m_message;
	};
}
