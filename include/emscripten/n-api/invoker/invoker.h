#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/value.h>

NS_NAPI_BEGIN

template<typename ReturnType, typename... Args>
struct Invoker {};

template<typename ReturnType>
struct Invoker <ReturnType>
{
	inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)()) {
		return fn();
	}

};

template<typename ReturnType, typename T0>
struct Invoker <ReturnType, T0>
{
	inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0)) {
		return fn(
			napi::value<T0>().get(env, argv[0]),
		);
	}
};

template<typename ReturnType, typename T0, typename T1>
struct Invoker <ReturnType, T0, T1>
{
	inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0, T1)) {

		return fn(
			napi::value<T0>().get(env, argv[0]),
			napi::value<T1>().get(env, argv[1])
		);
	}
};

template<typename ReturnType, typename T0, typename T1, typename T2>
struct Invoker <ReturnType, T0, T1, T2>
{
	inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0, T1, T2)) {

		return fn(
			napi::value<T0>().get(env,argv[0]),
			napi::value<T1>().get(env,argv[1]),
			napi::value<T2>().get(env,argv[2])

		);
	}
};


NS_NAPI_END
#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_INVOKER_H_