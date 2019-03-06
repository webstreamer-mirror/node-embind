#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/value.h>




#ifndef _arg
#define _arg(n) napi::value<T##n>(ctx,ctx.argv[n]).value()
#endif

NS_NAPI_BEGIN

template<typename ReturnType, typename... Args>
struct Invoker {};

template<typename ReturnType>
struct Invoker <ReturnType>
{
	inline static ReturnType invoke(const context_t& ctx, ReturnType(*fn)()) {
		return fn();
	}

};

template<typename ReturnType, typename T0>
struct Invoker <ReturnType, T0>
{
	inline static ReturnType invoke(const context_t& ctx, ReturnType(*fn)(T0)) {
		return fn(_arg(0));
	}
};

template<typename ReturnType, typename T0, typename T1>
struct Invoker <ReturnType, T0, T1>
{
	inline static ReturnType invoke(const context_t& ctx, ReturnType(*fn)(T0, T1)) {

		return fn(_arg(0),_arg(1));
	}
};

template<typename ReturnType, typename T0, typename T1, typename T2>
struct Invoker <ReturnType, T0, T1, T2>
{
	inline static ReturnType invoke(const context_t& ctx, ReturnType(*fn)(T0, T1, T2)) {

		return fn(_arg(0), _arg(1), _arg(2));
	}
};


NS_NAPI_END
#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_