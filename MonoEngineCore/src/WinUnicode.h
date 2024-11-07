#pragma once
#include <algorithm>

namespace utf_detail
{
	struct wide_to_multi_byte
	{
		explicit wide_to_multi_byte(UINT codepage)
			: _codepage(codepage)
		{}

		int get_length(const wchar_t* wide, size_t length) const
		{
			return ::WideCharToMultiByte(_codepage, 0, wide, static_cast<int>(length),
				nullptr, 0, nullptr, nullptr);
		}

		template<typename NarrowString>
		void convert(const wchar_t* wide, size_t length, NarrowString& dest) const
		{
			::WideCharToMultiByte(_codepage, 0, wide, static_cast<int>(length),
				&dest[0], static_cast<int>(dest.size()), nullptr, nullptr);
		}

	private:
		UINT _codepage;
	};

	struct multi_byte_to_wide
	{
		int get_length(const char* utf8, size_t length) const
		{
			return ::MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(length), nullptr, 0);
		}

		template<typename WideString>
		void convert(const char* utf8, size_t length, WideString& dest) const
		{
			::MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(length), &dest.front(), static_cast<int>(dest.size()));
		}
	};

	template<typename SourceString, typename DestString, typename Converter>
	inline void ChangeStringFormat(const SourceString& src, size_t len, DestString& dest, Converter func)
	{
		if (0u == len)
		{
			return dest.clear();
		}

		const int nChars = func.get_length(src, len);
		if (nChars <= 0)
		{
			return dest.clear();
		}
		else
		{
			dest.resize(nChars);
			return func.convert(src, len, dest);
		}
	}
}

template<typename DestString>
inline void ConvertWideStringToUTF8(const wchar_t* wide, DestString& utf8)
{
	if (wide)
		utf_detail::ChangeStringFormat(wide, wcslen(wide), utf8, utf_detail::wide_to_multi_byte(CP_UTF8));
	else
		utf8.clear();
}

template<typename DestString>
inline void ConvertUTF8ToWideString(const char* utf8, DestString& wide)
{
	if (utf8)
	{
		utf_detail::ChangeStringFormat(utf8, strlen(utf8), wide, utf_detail::multi_byte_to_wide());
		std::replace(wide.begin(), wide.end(), L'/', L'\\'); // Convert separators to Windows
	}
	else
		wide.clear();
}