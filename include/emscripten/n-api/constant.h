
#pragma once

#include "common.h"

namespace emscripten {
	namespace internal {


		struct constant_t
		{
			constant_t() :name(nullptr), value(nullptr), 
				constant_value(nullptr), function(nullptr)
			{}
			const char* name;
			napi_value  value;
			void*       constant_value;
			void*       function;
		};


		// ========================================
		//   Constant
		// ========================================

		template<typename ConstantType>
		struct Constant {

			ConstantType* value;
			napi_ref ref;

			static constant_t* prototype;

			static void Destructor(napi_env env, void* obj, void* finalize_hint)
			{
				//MetaClass<ClassType>* self = static_cast<MetaClass<ClassType>*>(obj);
				//delete self;
			}

			static napi_value New(napi_env env, napi_callback_info info)
			{
				NodeJS::Scope scope = NodeJS::Scope(env);
				return nullptr;
			}


			inline static napi_value napi_getter(napi_env env, napi_callback_info info) {
				napi_context_t ctx;
				ctx.env = env;
				ctx.argv = nullptr;
				auto scope = NodeJS::Scope(env);
				
				napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
			
				constant_t* c = static_cast<constant_t*>(ctx.data);
				if (c->value==nullptr) {
					c->value = napi::value<ConstantType>(env).napi_cast(*(ConstantType*)c->constant_value);
				}
				return c->value;
			}
		};


		inline void Register(std::vector<napi_property_descriptor>& props, constant_t* prototype, napi_env env, napi_value exports)
		{
			napi_property_descriptor prop = {
				prototype->name,
				nullptr,
				nullptr, //method;
				(napi_callback)prototype->function,
				nullptr, //setter;
				nullptr,
				napi_default,
				prototype
			};
			props.push_back(prop);
		}

	}
}
