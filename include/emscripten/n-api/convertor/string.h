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

		String(napi_env env, ::napi_value val)
		{
			napi_status status;
			size_t len = 0;

			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				status = ::napi_get_value_string_utf8(env, val, nullptr, 0, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);

				value.resize(len);
				status = ::napi_get_value_string_utf8(env, val, (char*)value.data(), value.size()+1, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			}
			else {
				status = ::napi_get_value_string_latin1(env, val, nullptr, 0, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);

				value.resize(len);
				status = ::napi_get_value_string_latin1(env, val, (char*)value.data(), value.size(), &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			}
		}

		inline static ::napi_value napi_value(napi_env env, type val) {
			napi_status status;
			::napi_value res;
			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				status = ::napi_create_string_utf8(env, val.data(), val.size(), &res);
			}
			else {
				status = ::napi_create_string_latin1(env, val.data(), val.size(), &res);
			}
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}
	};



	template<typename T>
	struct String<const T> : public String<T> {
		String(napi_env env, ::napi_value val)
			: String<T>(napi_env env, ::napi_value val)
		{}
	};

	template<typename T>
	struct String<T&> : public String<T> {
		String(napi_env env, ::napi_value val)
			: String<T>(env, val)
		{}
	};

	template<typename T>
	struct String<const T&> : public String<T> {
		String(napi_env env, ::napi_value val)
			: String<T>(env,val)
		{}
	};

	template<typename T>
	struct String<T&&> {
		String(napi_env env, ::napi_value val)
			: String<T>(env, val)
		{}

	};

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_