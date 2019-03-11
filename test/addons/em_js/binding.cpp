
#include <emscripten/em_js.h>
using namespace emscripten;

#include <time.h>
#include <iostream>

namespace em_js{

EM_JS(void, noarg, (void), { out("no args works"); });

EM_JS(int, noarg_int, (void), {
  out("no args returning int");
  return 12;
	});
EM_JS(double, noarg_double, (void), {
  out("no args returning double");
  return 12.25;
	});
EM_JS(void, intarg, (int x), { out("  takes ints: " + x); });
EM_JS(void, doublearg, (double d), { out("  takes doubles: " + d); });
#if 0
EM_JS(double, stringarg, (char* str), {
  out("  takes strings: " + Pointer_stringify(str));
  return 7.75;
	});
#endif
EM_JS(int, multi_intarg, (int x, int y), {
  out("  takes multiple ints: " + x + ", " + y);
  return 6;
	});
#if 0
EM_JS(double, multi_mixedarg, (int x, const char* str, double d), {
  out("  mixed arg types: " + x + ", " + Pointer_stringify(str) + ", " + d);
  return 8.125;
	});
#endif
EM_JS(int, unused_args, (int unused), {
  out("  ignores unused args");
  return 5.5;
	});
EM_JS(double, skip_args, (int x, int y), {
  out("  skips unused args: " + y);
  return 6;
	});
EM_JS(double, add_outer, (double x, double y, double z), {
  out("  " + x + " + " + z);
  return x + z;
	});
EM_JS(int, user_separator, (void), {
  out("  can use <::> separator in user code");
  return 15;
	});
#if 0
EM_JS(int, user_comma, (void), {
  var x, y;
  x = {};
  y = 3;
  x[y] = [1, 2, 3];
  out("  can have commas in user code: " + x[y]);
  return x[y][1];
	});

EM_JS(const char*, return_utf8_str, (void), {
	var jsString = '����ˤ���';
	var lengthBytes = lengthBytesUTF8(jsString);
	var stringOnWasmHeap = _malloc(lengthBytes);
	stringToUTF8(jsString, stringOnWasmHeap, lengthBytes + 1);
	return stringOnWasmHeap;
	});

EM_JS(const char*, return_str, (void), {
  var jsString = 'hello from js';
  var lengthBytes = jsString.length + 1;
  var stringOnWasmHeap = _malloc(lengthBytes);
  stringToUTF8(jsString, stringOnWasmHeap, lengthBytes + 1);
  return stringOnWasmHeap;
	});

#endif
};


struct embindEMJS
{

  static void noarg(void)
  {
    em_js::noarg();
  }

  static int noarg_int (void)
  {
    return em_js::noarg_int();
  }

  static double noarg_double (void)
  {
    return em_js::noarg_double();
  }

  static void intarg(int x)
  {
    em_js::intarg(x);
  }
  static void doublearg(double d)
  {
    em_js::doublearg(d);
  }

  static int multi_intarg(int x, int y)
  {
    return em_js::multi_intarg(x, y);
  }

  static int unused_args(int unused)
  {
    return em_js::unused_args(unused);
  }

  static double skip_args (int x, int y)
  {
    return em_js::skip_args(x, y);
  }

  static double add_outer (double x, double y, double z)
  {
    return em_js::add_outer(x, y, z);
  }

  static int user_separator (void)
  {
    return em_js::user_separator();
  }
};



EMSCRIPTEN_BINDINGS(binding)
{
	#if 0 // THIS NOT PERMITED
	function("noarg_double", em_js::noarg_double);
	function("noarg", em_js::noarg);
	function("noarg_int", em_js::noarg_int);
	function("intarg", em_js::intarg);
	function("doublearg", em_js::doublearg);
	function("multi_intarg", em_js::multi_intarg);
	function("unused_args", em_js::unused_args);
	function("skip_args", em_js::skip_args);
	function("add_outer", em_js::add_outer);
	function("user_separator", em_js::user_separator);
  #endif

	class_<embindEMJS>("EMJS")
        
		.class_function("noarg_double", &embindEMJS::noarg_double)
		.class_function("noarg", &embindEMJS::noarg)
		.class_function("noarg_int", &embindEMJS::noarg_int)
		.class_function("intarg", &embindEMJS::intarg)
		.class_function("doublearg", &embindEMJS::doublearg)
		.class_function("multi_intarg", &embindEMJS::multi_intarg)
		.class_function("unused_args", &embindEMJS::unused_args)
		.class_function("skip_args", &embindEMJS::skip_args)
		.class_function("add_outer", &embindEMJS::add_outer)
		.class_function("user_separator", &embindEMJS::user_separator)
		;
}



#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif