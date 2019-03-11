/*
 * Copyright 2018 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifndef __em_js_h__
#define __em_js_h__
#include <emscripten/bind.h>
#include <emscripten/n-api/jscript.h>
#include <emscripten/n-api/jfunction.h>

// EM_JS declares JS functions in C code.
// Example uses can be found in tests/core/test_em_js.cpp

// Implementation details:

// The EM_JS macro is specified as:
//   EM_JS(return type, function name, (arguments), {body})
// but the macro uses __VA_ARGS__ instead of a fourth argument. This is so that
// function bodies containing commas are seamlessly handled.

// EM_JS declares the JS function with a C function prototype, which becomes a
// function import in asm.js/wasm. It also declares an __em_js__-prefixed
// function, which we can use to pass information to the Emscripten compiler
// that survives going through LLVM.
// Example:
//   EM_JS(int, foo, (int x, int y), { return 2 * x + y; })
// would get translated into:
//   int foo(int x, int y);
//   __attribute__((used, visibility("default")))
//   const char* __em_js__foo() {
//     return "(int x, int y)<::>{ return 2 * x + y; }";
//   }
// We pack the arguments and function body into a constant string so it's
// readable from asm.js/wasm post-processing.
// Later we see a function called __em_js__foo, meaning we need to create a JS
// function:
//   function foo(x, y) { return 2 * x + y; }
// We use <::> to separate the arguments from the function body because it isn't
// valid anywhere in a C function declaration.

// Generated __em_js__-prefixed functions are read by either the JSBackend (for
// asm.js) or by Binaryen, and the string data is extracted into the Emscripten
// metadata dictionary under the "emJsFuncs" key. emJsFuncs itself is a
// dictionary where the keys are function names (not prefixed with __em_js__),
// and the values are the <::>-including description strings.

// emJsFuncs metadata is read in emscripten.py's create_em_js, which creates an
// array of JS function strings to be included in the JS output.


#define EM_JS(ret, name, params, code)                                                            \
namespace emscripten_{ namespace em_js { namespace internal{                                      \
template<typename ReturnType, typename... Args> 										          \
struct ____##name##_JFunction {};               										          \
template<typename ReturnType>                   										          \
struct ____##name##_JFunction<ReturnType>                                                         \
        : public ::emscripten::internal::napi::_JFunction<ReturnType> {    		                      \
	static ReturnType function() {														          \
		return ::emscripten::internal::napi::_JFunction<ReturnType>::Call(#params, #code);	          \
	}																					          \
};																						          \
template<typename ReturnType, typename T0>												          \
struct ____##name##_JFunction <ReturnType, T0>                                                    \
        : public ::emscripten::internal::napi::_JFunction<ReturnType> {		                          \
	static ReturnType function(T0 v0) {													          \
		return ::emscripten::internal::napi::_JFunction<ReturnType                                      \
		         >::Call(#params, #code, v0);					                                  \
	}																					          \
};																						          \
template<typename ReturnType, typename T0, typename T1>									          \
struct ____##name##_JFunction <ReturnType, T0, T1>                                                \
         : public ::emscripten::internal::napi::_JFunction<ReturnType> {	                              \
	static ReturnType function(T0 v0, T1 v1) {										              \
		return ::emscripten::internal::napi::_JFunction<ReturnType                                      \
		         >::Call(#params, #code, v0,v1);		   	                                      \
	}																				              \
};																						          \
template<typename ReturnType>                                                                     \
inline static void* ____##name##_function(ReturnType(*f)()) {							          \
	return (void*)____##name##_JFunction<ReturnType>::function;							          \
}																						          \
template<typename ReturnType, typename T0>												          \
inline static void* ____##name##_function(ReturnType(*f)(T0)) {							          \
	return (void*)____##name##_JFunction<ReturnType, T0>::function;						          \
}																						          \
template<typename ReturnType, typename T0, typename T1>									          \
inline static void* ____##name##_function(ReturnType(*f)(T0, T1)) {						          \
	return (void*)____##name##_JFunction<ReturnType, T0, T1>::function;					          \
}																						          \
template<typename ReturnType, typename T0, typename T1, typename T2>					          \
inline static void* ____##name##_function(ReturnType(*f)(T0, T1, T2)) {					          \
	return (void*)____##name##_JFunction<ReturnType, T0, T1, T2>::function;				          \
}                                                                                                 \
typedef ret(*____##name##_function_type) params;                                                  \
}}}/*!emscripten_::em_js::internal*/                                                              \
static  ret (* name) params = (emscripten_::em_js::internal::____##name##_function_type)          \
        emscripten_::em_js::internal::____##name##_function(                                      \
              (emscripten_::em_js::internal::____##name##_function_type)0 );                      
                                                                                                  

#endif // __em_js_h__
