#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_

#include <emscripten/n-api/common.h>


NS_NAPI_BEGIN

namespace convertor {

	template<typename T>
	struct Fundamental;

	template<>
	struct Fundamental <bool> {
		typedef bool type;
		type     value;

		Fundamental(napi_env env, napi_value val)
		{
			napi_status status;
			status = napi_get_value_bool(env, val, &this->value);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			status  = napi_get_boolean(env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}
	};


	template<>
	struct Fundamental <int> {
		typedef int type;
		type     value;
		Fundamental(napi_env env, napi_value val)
		{
			napi_status status;
			if (sizeof(int) == 64) {
				status = napi_get_value_int64(env, val, (int64_t*)&this->value);
			}
			else {
				status = napi_get_value_int32(env, val, (int32_t*)&this->value);
			}

			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			if (sizeof(int) == 64) {
				status = napi_create_int64(env, (int64_t)val, &res);
			}
			else {
				status = napi_create_int32(env, (int32_t)val, &res);
			}
			
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);

			return res;
		}
	};

	template<>
	struct Fundamental <float> {
		typedef float type;
		double     value;

		Fundamental(napi_env env, napi_value val)
		{
			napi_status status;
			status =napi_get_value_double(env, val, &this->value);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			status =napi_create_double(env, (double)val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}

	};

	template<>
	struct Fundamental <double> {
		typedef double type;
		type     value;

		Fundamental(napi_env env, napi_value val)
		{
			napi_status status;
			status = napi_get_value_double(env, val, &this->value);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_double(env, (double)val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}

	};


	template<>
	struct Fundamental <int64_t> {
		typedef int64_t type;
		type     value;

		Fundamental(napi_env env, napi_value val) {
			napi_status status;
			status = napi_get_value_int64(env, val, &this->value);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_int64(env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}

	};

	template<>
	struct Fundamental <uint64_t> {
		typedef uint64_t type;
		type     value;

		Fundamental(napi_env env, napi_value val) {
			napi_status status;
			status = napi_get_value_int64(env, val, (int64_t*)&this->value);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
		}

		inline static napi_value create(napi_env env, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_int64(env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
			return res;
		}

	};

	template<typename T>
	struct Fundamental<const T> : public Fundamental<T> {
		Fundamental(napi_env env)
			: Fundamental<T>(env)
		{}
	};

	template<typename T>
	struct Fundamental<T&> : public Fundamental<T> {
		Fundamental(napi_env env)
			: Fundamental<T>(env)
		{}
	};

	template<typename T>
	struct Fundamental<const T&> : public Fundamental<T> {
		Fundamental(napi_env env)
			: Fundamental<T>(env)
		{}
	};

	template<typename T>
	struct Fundamental<T&&> {
		typedef typename Fundamental<T>::type type;
		Fundamental<T> obj_;
		Fundamental(napi_env env)
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

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_