
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

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
	function("int_to_string", select_overload<std::string(int)>(std::to_string));
	function("double_to_string", select_overload<std::string(double)>(std::to_string));

}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
