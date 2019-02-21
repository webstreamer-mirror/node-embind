#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_

#include <emscripten/n-api/common.h>


NS_NAPI_BEGIN



template<typename T>
struct FundamentalConvertor;

template<>
struct FundamentalConvertor <bool> {
	typedef bool type;
	type     value_;

	inline type get(napi_env env, napi_value val)
	{
		napi_get_value_bool(env, val, &value_);
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		napi_get_boolean(env, val, &res);
		return res;
	}
};


template<>
struct FundamentalConvertor <int> {
	typedef int type;
	type     value_;
	inline type get(napi_env env, napi_value val)
	{
		if (sizeof(int) == 64) {
			napi_get_value_int64(env, val, (int64_t*)&value_);
		}
		else {
			napi_get_value_int32(env, val, (int32_t*)&value_);
		}
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		if (sizeof(int) == 64) {
			napi_create_int64(env, (int64_t)val, &res);
		}
		else {
			napi_create_int32(env, (int32_t)val, &res);
		}
		return res;
	}
};

template<>
struct FundamentalConvertor <float> {
	typedef float type;
	double     value_;

	inline type get(napi_env env, napi_value val)
	{
		napi_get_value_double(env, val, &value_);
		return static_cast<type>(value_);
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		napi_create_double(env, (double)val, &res);
		return res;
	}

};

template<>
struct FundamentalConvertor <double> {
	typedef double type;
	type     value_;

	inline type get(napi_env env, napi_value val)
	{
		napi_get_value_double(env, val, &value_);
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		napi_create_double(env, (double)val, &res);
		return res;
	}

};


template<>
struct FundamentalConvertor <int64_t> {
	typedef int64_t type;
	type     value_;

	inline type get(napi_env env, napi_value val) {
		napi_get_value_int64(env, val, &value_);
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		napi_create_int64(env, val, &res);
		return res;
	}

};

template<>
struct FundamentalConvertor <uint64_t> {
	typedef uint64_t type;
	type     value_;

	inline type get(napi_env env, napi_value val) {
		napi_get_value_int64(env, val, (int64_t*)&value_);
		return value_;
	}

	inline static napi_value create(napi_env env, type val) {
		napi_value res;
		napi_create_int64(env, val, &res);
		return res;
	}

};

template<typename T>
struct FundamentalConvertor<const T> : public FundamentalConvertor<T> {
	FundamentalConvertor(napi_env env)
		: FundamentalConvertor<T>(env)
	{}
};

template<typename T>
struct FundamentalConvertor<T&> : public FundamentalConvertor<T> {
	FundamentalConvertor(napi_env env)
		: FundamentalConvertor<T>(env)
	{}
};

template<typename T>
struct FundamentalConvertor<const T&> : public FundamentalConvertor<T> {
	FundamentalConvertor(napi_env env)
		: FundamentalConvertor<T>(env)
	{}
};

template<typename T>
struct FundamentalConvertor<T&&> {
	typedef typename FundamentalConvertor<T>::type type;
	FundamentalConvertor<T> obj_;
	FundamentalConvertor(napi_env env)
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