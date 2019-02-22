
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

int _value = 0;
void Set(int v) { _value = v; }
int  Get() { return _value; }

EMSCRIPTEN_BINDINGS(binding)
{
    using namespace emscripten;
    function("Inc", &Inc);
    function("intSum", select_overload<int(int,int)>(&Sum));
    function("intSum", select_overload<int(int, int,int)>(&Sum));
	function("doubleSum", select_overload<double(double, double)>(&Sum));
	function("doubleSum", select_overload<double(double, double, double)>(&Sum));
	function("Set",Set);
	function("Get",Get);
}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
