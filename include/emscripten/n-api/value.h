
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_
#include <emscripten/n-api/common.h>
#include <emscripten/n-api/convertor/fundamental.h>
#include <emscripten/n-api/convertor/string.h>

NS_NAPI_BEGIN


template <typename T>
struct Convertor {
	using _T = typename nomakeup<T>::type;
	
	using type = 
		typename std::conditional< std::is_fundamental<_T>::value, typename napi::FundamentalConvertor<T>, 
		
		typename std::conditional< std::is_same<_T,std::string>::value, typename napi::StringConvertor<T>,

		void>::type

	>::type;

	static_assert(!std::is_void<type>::value, 
	"====  Unsupported type in Convertor (support list as below)  ====\n"\
	"1. fundamental\n"\
	"2. std::string\n"
	);
};


template <typename T>
using value = typename Convertor<T>::type;

NS_NAPI_END

#endif //!_NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_