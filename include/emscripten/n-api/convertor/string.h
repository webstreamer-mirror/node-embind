#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_STRING_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_STRING_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/module.h>


NS_NAPI_BEGIN


template<typename T>
struct StringConvertor;

template<>
struct StringConvertor <std::string> {
	typedef std::string type;
	napi_env env_;
	type     value_;
	StringConvertor(napi_env env)
		: env_(env)
	{}

	inline type cast(napi_env env, napi_value val)
	{
		napi_value res;
		size_t len = 0;
	
		if (Module::EMBIND_STD_STRING_IS_UTF8) {
			::napi_get_value_string_utf8(env, val, nullptr, 0, &len);
			value_.resize(len);
			::napi_get_value_string_utf8(env, val, (char*)value_.data(), value_.size(), &len);
			assert(value_.size() == len);
		}
		else {
			::napi_get_value_string_latin1(env, val, nullptr, 0, &len);
			value_.resize(len);
			::napi_get_value_string_latin1(env, val, (char*)value_.data(), value_.size(), &len);
			assert(value_.size() == len);
		}
		napi_get_boolean(env_, val, &res);
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		if (Module::EMBIND_STD_STRING_IS_UTF8) {
			::napi_create_string_utf8(env, val.data(),val.size(),&res);
		}
		else {
			::napi_create_string_latin1(env, val.data(), val.size(), &res);
		}
		return res;
	}
};



template<typename T>
struct StringConvertor<const T> : public StringConvertor<T> {
	StringConvertor(napi_env env)
		: StringConvertor<T>(env)
	{}
};

template<typename T>
struct StringConvertor<T&> : public StringConvertor<T> {
	StringConvertor(napi_env env)
		: StringConvertor<T>(env)
	{}
};

template<typename T>
struct StringConvertor<const T&> : public StringConvertor<T> {
	StringConvertor(napi_env env)
		: StringConvertor<T>(env)
	{}
};

template<typename T>
struct StringConvertor<T&&> {
	typedef typename StringConvertor<T>::type type;
	StringConvertor<T> obj_;
	StringConvertor(napi_env env)
		: obj_(env)
	{}

	inline type cast(napi_value val)
	{
		return obj_.cast(val);
	}

	inline napi_value napi_value_(type val) {
		return obj_.napi_value_(val);
	}
};


NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_