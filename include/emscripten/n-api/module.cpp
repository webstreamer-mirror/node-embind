
#ifndef _IN_EMSCRIPTEN_NODE_EMBIND_FILE_
#error emscripten/n-api/module.cpp can only be include in <emscripten/node-embind>
#endif

#include <emscripten/bind.h>



NS_NAPI_BEGIN


module_t Module::module_;
napi_env VM::env_ = nullptr;


//====================================
//       Registers
//====================================

void register_function(
	const char* name,
	unsigned argCount,
	const TYPEID argTypes[],
	GenericFunction invoker,
	GenericFunction function)
{
	module_t& m = Module::module_;
	function_t* f = new function_t();
	f->name = name;
	f->argc = argCount - 1;
	typedef napi_value(*Fn)(const function_t*, const context_t&);

	f->invoke = (Fn)invoker;

	f->function = function;
	m.functions.push_back(f);
}


NS_NAPI_END



napi_value Init(napi_env env, napi_value exports) {

  return exports;
}


NAPI_MODULE(NODE_GYP_MODULE_NAME, emscripten::internal::napi::Module::Init);

