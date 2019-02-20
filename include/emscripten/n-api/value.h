
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_
#include <emscripten/n-api/common.h>
#include <emscripten/n-api/convertor/fundamental.h>

NS_NAPI_BEGIN


template <typename T>
struct _Convertor {
	using _T = typename nomakeup<T>::type;
	
	using typex = typename std::conditional< true, int, void>::type;
	using type = typename std::conditional< std::is_fundamental<_T>::value, typename napi::Fundamental<T>, void>::type;



	static_assert(!std::is_void<type>::value, "====  Unsupported type in Convertor (support list as below)  ====\n"\
		"1. fundamental\n"		
		);
};



//template<typename T>
//using Convertor = _Convertor<T>::type;


template<typename T>
struct _Value : public _Convertor<T>::type
{
	using _Base = typename _Convertor<T>::type;
	_Value(napi_env env)
		: _Base(env)
	{}
};


template <typename T>
using value = _Value<T>;

NS_NAPI_END

#endif //!_NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_