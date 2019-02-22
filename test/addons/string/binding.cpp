
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>


#ifdef _MSC_VER
bool WideStringToString(const std::wstring& src, std::string &str)
{
	std::locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (src.size() + 1) * wchar_size);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t out_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		str = data_to;
		delete[] data_to;
		return true;
	}
	delete[] data_to;
	return false;
}

bool StringToWideString(const std::string& src, std::wstring &wstr)
{
	std::locale sys_locale("");
	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, src.size() + 1);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t in_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		wstr = data_to;
		delete[] data_to;
		return true;
	}
	delete[] data_to;
	return false;
}


bool WCharStringToUTF8String(const std::wstring &wstr, std::string &u8str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	u8str = conv.to_bytes(wstr);
	return true;
}

bool UTF8StringToWCharString(const std::string &u8str, std::wstring &wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	wstr = conv.from_bytes(u8str);
	return true;
}

std::string wstringToUTF8String(const std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

#define _T(x) wstringToUTF8String((wchar_t*)"\0x00E1")
#else
#define _T(x) wstringToUTF8String(x)
#endif

std::string get_non_ascii_string(bool embindStdStringUTF8Support) {
	if (embindStdStringUTF8Support) {

		//ASCII
		std::string testString{ "aei" };

		//Latin-1 Supplement
		testString += "\u00E1\u00E9\u00ED";
		//Greek
		testString += "\u03B1\u03B5\u03B9";
		//Cyrillic
		testString += "\u0416\u041B\u0424";
		//CJK
		testString += "\u5F9E\u7345\u5B50";
		//Euro sign
		testString += "\u20AC";
		return testString;
	}
	else {
		char c[128 + 1];
		c[128] = 0;
		for (int i = 0; i < 128; ++i) {
			c[i] = 128 + i;
		}
		return c;
	}
}


std::string nop(const std::string& s) {
	return s;
}
EMSCRIPTEN_BINDINGS(binding)
{
    using namespace emscripten;
	
	function("get_non_ascii_string", get_non_ascii_string);
	function("nop", nop);

}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
