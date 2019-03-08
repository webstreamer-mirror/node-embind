#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_


#include <memory>
#include <malloc.h>

#include <list>
#include <map>
#include <vector>

#include <node_api.h>

#define EMSCRIPTEN_ALWAYS_INLINE inline

#define NS_NAPI_BEGIN namespace emscripten { namespace internal { namespace napi {
#define NS_NAPI_END   }}}

NS_NAPI_BEGIN
typedef void* class_ptr;


template<typename T>
struct nomakeup {

	typedef 
		typename std::remove_cv < 		
		typename std::remove_reference<
		typename std::remove_pointer<
		T
		>::type
		>::type
		>::type
		type;
};


struct context_t {
	context_t()
		: data(nullptr), env(nullptr), argc(0), argv(nullptr), js(nullptr)
	{}
	void* data;
	napi_env env;
	size_t argc;
	napi_value* argv;
	napi_value js;

};



struct VM {

    inline static napi_env get_napi_env() { return context_ ? context_->env : nullptr; }

    //
    // internal
    //
    inline static void context(context_t* ctx) { context_ = ctx; }
    inline static context_t* context() { return context_; }
    static context_t* context_;

};




NS_NAPI_END

//#define __env__ VM::get_napi_env();



#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_