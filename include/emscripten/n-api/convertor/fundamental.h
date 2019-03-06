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
		type     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val)
		{
			napi_status status;
			status = napi_get_value_bool(ctx.env, val, &this->value_);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			status  = napi_get_boolean(ctx.env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}
	};


	template<>
	struct Fundamental <int> {
		typedef int type;
		type     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val)
		{
			napi_status status;
			if (sizeof(int) == 64) {
				status = napi_get_value_int64(ctx.env, val, (int64_t*)&this->value_);
			}
			else {
				status = napi_get_value_int32(ctx.env, val, (int32_t*)&this->value_);
			}

			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			if (sizeof(int) == 64) {
				status = napi_create_int64(ctx.env, (int64_t)val, &res);
			}
			else {
				status = napi_create_int32(ctx.env, (int32_t)val, &res);
			}
			
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

			return res;
		}
	};

	template<>
	struct Fundamental <float> {
		typedef float type;
		double     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val)
		{
			napi_status status;
			status =napi_get_value_double(ctx.env, val, &this->value_);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			status =napi_create_double(ctx.env, (double)val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}

	};

	template<>
	struct Fundamental <double> {
		typedef double type;
		type     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val)
		{
			napi_status status;
			status = napi_get_value_double(ctx.env, val, &this->value_);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_double(ctx.env, (double)val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}

	};


	template<>
	struct Fundamental <int64_t> {
		typedef int64_t type;
		type     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val) {
			napi_status status;
			status = napi_get_value_int64(ctx.env, val, &this->value_);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_int64(ctx.env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}

	};

	template<>
	struct Fundamental <uint64_t> {
		typedef uint64_t type;
		type     value_;
        inline type value() { return value_; }

		Fundamental(const context_t& ctx, napi_value val) {
			napi_status status;
			status = napi_get_value_int64(ctx.env, val, (int64_t*)&this->value_);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
		}

		inline static napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			napi_value res;
			status = napi_create_int64(ctx.env, val, &res);
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}

	};

	template<typename T>
	struct Fundamental<const T> : public Fundamental<T> {
		Fundamental(const context_t& ctx, napi_value val)
			: Fundamental<T>(ctx, val)
		{}
	};

	template<typename T>
	struct Fundamental<T&> : public Fundamental<T> {
		Fundamental(const context_t& ctx, napi_value val)
			: Fundamental<T>(ctx, val)
		{}
	};

	template<typename T>
	struct Fundamental<const T&> : public Fundamental<T> {
		Fundamental(const context_t& ctx, napi_value val)
			: Fundamental<T>(ctx, val)
		{}
	};

    template<typename T>
    struct Fundamental<T&&> : public Fundamental<T> {
        Fundamental(const context_t& ctx, napi_value val)
            : Fundamental<T>(ctx,val)
        {}
    };

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_