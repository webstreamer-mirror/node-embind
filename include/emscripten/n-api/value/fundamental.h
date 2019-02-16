#pragma once

#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <node_api.h>


namespace emscripten {
	namespace napi {
		namespace internal {

			template<typename T>
			struct Fundamental;

			//template<>
			//struct Fundamental <void> {
			//	typedef bool type;
			//	napi_env env_;
			//	type     value_;
			//	Fundamental(napi_env env)
			//		: env_(env)
			//	{}
			//
			//	inline void native_cast(napi_value val)
			//	{
			//	}
			//
			//	inline napi_value napi_cast(bool val) {
			//		napi_value res;
			//		napi_get_boolean(env_, val, &res);
			//		return res;
			//	}
			//};

			template<>
			struct Fundamental <bool> {
				typedef bool type;
				napi_env env_;
				type     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline bool native_cast(napi_value val)
				{
					napi_get_value_bool(env_, val, &value_);
					return value_;
				}

				inline napi_value napi_cast(bool val) {
					napi_value res;
					napi_get_boolean(env_, val, &res);
					return res;
				}
			};


			template<>
			struct Fundamental <int> {
				typedef int type;
				napi_env env_;
				type     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					if (sizeof(int) == 64) {
						napi_get_value_int64(env_, val, (int64_t*)&value_);
					}
					else {
						napi_get_value_int32(env_, val, (int32_t*)&value_);
					}
					return value_;
				}

				inline napi_value napi_cast(type val) {
					napi_value res;
					if (sizeof(int) == 64) {
						napi_create_int64(env_, (int64_t)val, &res);
					}
					else {
						napi_create_int32(env_, (int32_t)val, &res);
					}
					return res;
				}
			};

			template<>
			struct Fundamental <float> {
				typedef float type;
				napi_env env_;
				double     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					napi_get_value_double(env_, val, &value_);
					return static_cast<type>(value_);
				}

				inline napi_value napi_cast(type val) {
					napi_value res;
					napi_create_double(env_, (double)val, &res);
					return res;
				}

			};

			template<>
			struct Fundamental <double> {
				typedef double type;
				napi_env env_;
				type     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					napi_get_value_double(env_, val, &value_);
					return value_;
				}

				inline napi_value napi_cast(type val) {
					napi_value res;
					napi_create_double(env_, (double)val, &res);
					return res;
				}

			};


			template<>
			struct Fundamental <int64_t> {
				typedef int64_t type;
				napi_env env_;
				type     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					napi_get_value_int64(env_, val, &value_);
					return value_;
				}

				inline napi_value napi_cast(type val) {
					napi_value res;
					napi_create_int64(env_, val, &res);
					return res;
				}

			};

			template<>
			struct Fundamental <uint64_t> {
				typedef uint64_t type;
				napi_env env_;
				type     value_;
				Fundamental(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					napi_get_value_int64(env_, val, (int64_t*)&value_);
					return value_;
				}

				inline napi_value napi_cast(type val) {
					napi_value res;
					napi_create_int64(env_, val, &res);
					return res;
				}

			};


			//




			////

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

				inline type native_cast(napi_value val)
				{
					return obj_.native_cast(val);
				}

				inline napi_value napi_cast(type val) {
					return obj_.napi_cast(val);
				}
			};


		}
	}
}