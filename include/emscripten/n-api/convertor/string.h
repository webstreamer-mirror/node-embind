#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_STRING_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_STRING_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/module.h>


NS_NAPI_BEGIN
namespace convertor {

	template<typename T>
	struct String;

	template<>
	struct String <std::string> {
		typedef std::string type;
		type     value;

		String(napi_env env, napi_value val)
		{
			size_t len = 0;

			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				::napi_get_value_string_utf8(env, val, nullptr, 0, &len);
				value.resize(len);
				::napi_get_value_string_utf8(env, val, (char*)value.data(), value.size()+1, &len);
			}
			else {
				::napi_get_value_string_latin1(env, val, nullptr, 0, &len);
				value.resize(len);
				::napi_get_value_string_latin1(env, val, (char*)value.data(), value.size(), &len);
			}
		}

		inline static napi_value create(napi_env env, type val) {
			napi_value res;
			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				::napi_create_string_utf8(env, val.data(), val.size(), &res);
			}
			else {
				::napi_create_string_latin1(env, val.data(), val.size(), &res);
			}
			return res;
		}
	};



	template<typename T>
	struct String<const T> : public String<T> {
		String(napi_env env, napi_value val)
			: String<T>(napi_env env, napi_value val)
		{}
	};

	template<typename T>
	struct String<T&> : public String<T> {
		String(napi_env env, napi_value val)
			: String<T>(napi_env env, napi_value val)
		{}
	};

	template<typename T>
	struct String<const T&> : public String<T> {
		String(napi_env env, napi_value val)
			: String<T>(env,val)
		{}
	};

	template<typename T>
	struct String<T&&> {
		String(napi_env env, napi_value val)
			: String<T>(napi_env env, napi_value val)
		{}

	};

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_