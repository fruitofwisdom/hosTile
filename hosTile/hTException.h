#pragma once

#include <exception>
#include <string>

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

	private:
		std::string m_message;
	};
}
