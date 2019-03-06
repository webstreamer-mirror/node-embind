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
        std::string value_;
        inline std::string& value() { return value_; }

		String(const context_t& ctx, ::napi_value val)
		{
			napi_status status;
			size_t len = 0;

			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				status = ::napi_get_value_string_utf8(ctx.env, val, nullptr, 0, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                value_.resize(len);
				status = ::napi_get_value_string_utf8(ctx.env, val, (char*)value_.data(), value_.size()+1, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			}
			else {
				status = ::napi_get_value_string_latin1(ctx.env, val, nullptr, 0, &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                value_.resize(len);
				status = ::napi_get_value_string_latin1(ctx.env, val, (char*)value_.data(), value_.size(), &len);
				NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			}
		}

		inline static ::napi_value napivalue(const context_t& ctx, type val) {
			napi_status status;
			::napi_value res;
			if (Module::EMBIND_STD_STRING_IS_UTF8) {
				status = ::napi_create_string_utf8(ctx.env, val.data(), val.size(), &res);
			}
			else {
				status = ::napi_create_string_latin1(ctx.env, val.data(), val.size(), &res);
			}
			NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
			return res;
		}
	};



	template<typename T>
	struct String<const T> : public String<T> {
		String(const context_t& ctx, ::napi_value val)
			: String<T>(ctx, val)
		{}
	};

	template<typename T>
	struct String<T&> : public String<T> {
		String(const context_t& ctx, ::napi_value val)
			: String<T>(ctx, val)
		{}
	};

	template<typename T>
	struct String<const T&> : public String<T> {
		String(const context_t& ctx, ::napi_value val)
			: String<T>(ctx,val)
		{}
	};

	template<typename T>
	struct String<T&&> {
		String(const context_t& ctx, ::napi_value val)
			: String<T>(ctx, val)
		{}

	};

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_