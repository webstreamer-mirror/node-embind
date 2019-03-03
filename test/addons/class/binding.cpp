
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

static int marker_=0;
int marker() { return marker_; }
void marker(int v) { marker_ = v; }

class Empty {
public:
	Empty() {
		marker_++;
	}
	~Empty() {
		marker_--;
	}
};

EMSCRIPTEN_BINDINGS(binding)
{
  using namespace emscripten;
  function("marker", select_overload<int(void)>(marker));
  function("marker", select_overload<void(int)>(marker));
  class_<Empty>("Empty")
	  .constructor<>();
	  
	  

}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
