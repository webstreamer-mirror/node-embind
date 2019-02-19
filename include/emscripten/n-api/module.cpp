
#ifndef _IN_EMSCRIPTEN_NODE_EMBIND_FILE_
#error emscripten/n-api/module.cpp can only be include in <emscripten/node-embind>
#endif
#include <emscripten/bind.h>



NS_NAPI_BEGIN


module_t Module::module_;
napi_env VM::env_ = nullptr;


NS_NAPI_END



NAPI_MODULE(NODE_GYP_MODULE_NAME, emscripten::internal::napi::Module::Init);

