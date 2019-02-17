
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_FUNCTION_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_FUNCTION_H_

#include <emscripten/n-api/common.h>

namespace emscripten {
	namespace internal {
		namespace napi {
			static napi_env cur_env(napi_env e = nullptr) {
				static napi_env env = nullptr;
				if (e) env = e;
				return env;
			}
			struct function_t {
				const char* name;
				size_t argc;
				void* function; // C++ function
				napi_value(*invoke)(const function_t*, const context_t&);
				function_t* next; // this only used when function overload
				napi_property_attributes attributes;

				function_t()
					:name(nullptr), argc(0), function(nullptr), invoke(nullptr),
					next(nullptr), attributes(napi_default)
				{}

				~function_t()
				{
					// release overload
					function_t* fn = this->next;
					while (fn) {
						function_t* tmp = fn->next;
						delete fn;
						fn = tmp;
					}
				}
			};

			inline static napi_value napi_method(napi_env env, napi_callback_info info) {
				context_t ctx;
				ctx.env = env;
				ctx.argv = nullptr;
				cur_env(env);

				//NodeJS::Scope scope = NodeJS::Scope(env);

				napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
				if (ctx.argc > 0)
				{
					ctx.argv = (napi_value*)alloca(sizeof(napi_value)*ctx.argc);
				}

				napi_get_cb_info(env, info, &ctx.argc, ctx.argv, &ctx.js, nullptr);
				function_t* self = static_cast<function_t*>(ctx.data);
				return self->invoke(self, ctx);
			}
		}
	}
}
#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_FUNCTION_H_