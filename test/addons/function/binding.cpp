
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>

#include <emscripten/bind.h>


/*===============================================*\
     Function
\* ==============================================*/


int Inc() { static int n = 0; return n++; }

int   Sum(int a, int b) { return a + b; }
int   Sum(int a, int b, int c) { return a + b + c; }
double Sum(double a, double b) { return a + b; }
double Sum(double a, double b, double c) { return a + b + c; }

std::string toUpper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
	return s;
}

std::string toLower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

using namespace emscripten;
EMSCRIPTEN_BINDINGS(binding)
{


    function("Inc", &Inc);
    function("intSum", select_overload<int(int,int)>(&Sum));
    function("intSum", select_overload<int(int, int,int)>(&Sum));
	function("doubleSum", select_overload<double(double, double)>(&Sum));
	function("doubleSum", select_overload<double(double, double, double)>(&Sum));

	function("toUpper", toUpper);

	
}


#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif

