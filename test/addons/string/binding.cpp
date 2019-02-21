
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>

#include <emscripten/bind.h>







EMSCRIPTEN_BINDINGS(binding)
{
    using namespace emscripten;
	int a = 99;
	std::cout << a << std::endl;
}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
