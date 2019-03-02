#pragma once

#include <emscripten/internal/common.h>

namespace emscripten {
	namespace internal {

		struct property_t {
			property_t()
				: name(nullptr),getter(nullptr),setter(nullptr)
				, setter_context(nullptr), getter_context(nullptr)
			{}
			const char* name;
			napi_value(*getter)(const property_t*, const napi_context_t&);
			napi_value(*setter)(const property_t*, const napi_context_t&);
			void* setter_context;
			void* getter_context;
		};

		inline static napi_value napi_setter(napi_env env, napi_callback_info info) {
			napi_context_t ctx;
			ctx.env = env;
			ctx.argv = nullptr;
			
			napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
			if (ctx.argc > 0)
			{
				ctx.argv = (napi_value*)alloca(sizeof(napi_value)*ctx.argc);
			}
			
			napi_get_cb_info(env, info, &ctx.argc, ctx.argv, &ctx.js, nullptr);
			property_t* self = static_cast<property_t*>(ctx.data);
			return self->setter(self, ctx);
		}

		inline static napi_value napi_getter(napi_env env, napi_callback_info info) {
			napi_context_t ctx;
			ctx.env = env;
			ctx.argv = nullptr;
			napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
			if (ctx.argc > 0)
			{
				ctx.argv = (napi_value*)alloca(sizeof(napi_value)*ctx.argc);
			}

			napi_get_cb_info(env, info, &ctx.argc, ctx.argv, &ctx.js, nullptr);
			property_t* self = static_cast<property_t*>(ctx.data);
			return self->getter(self,ctx);
		}

	}
}