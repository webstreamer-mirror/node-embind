
#pragma once

#include "common.h"
#include "function.h"
#include "property.h"
namespace emscripten {
	namespace internal {

		struct constructor_t {
			size_t argc;
			void* function; // C++ function
			class_ptr (*invoke)(const constructor_t*, const napi_context_t&);

			constructor_t()
				:argc(0), function(nullptr), invoke(nullptr)
			{}
		};

		struct class_t
		{
			class_t() :name(nullptr), New(nullptr), ref(nullptr)
			{}
			const char* name;
			std::list<constructor_t*> ctors;
			std::list<function_t*> function;
			std::list<property_t*> property;
			napi_ref ref;
			napi_value(*New)(napi_env env, napi_callback_info info);
		};

		inline void Initialize(class_t* prototype, napi_env env, napi_value exports)
		{
			std::vector<napi_property_descriptor> prop;
			napi_value ctor = nullptr;
			
			for (std::list<property_t*>::iterator it = prototype->property.begin();
				it != prototype->property.end(); it++) {

				napi_property_descriptor desc = {
					(*it)->name,
					nullptr,nullptr,
					(*it)->getter ? &napi_getter : nullptr,
					(*it)->setter ? &napi_setter : nullptr,
					0,/*napi_default*/(*it)->attributes,*it
				};
				prop.push_back(desc);
			}

			for (std::list<function_t*>::iterator it = prototype->function.begin();
				it != prototype->function.end(); it++) {
				napi_property_descriptor desc = {
					(*it)->name,
					nullptr,
					napi_method,
					nullptr,nullptr,
					0,/*napi_default*/(*it)->attributes,*it
				};
				prop.push_back(desc);
			}

			napi_define_class(env, prototype->name, -1, prototype->New, prototype, prop.size(), prop.data(), &ctor);
			napi_create_reference(env, ctor, 1, &(prototype->ref));
			napi_set_named_property(env, exports, prototype->name, ctor);
		}

		

		// ========================================
		//   Class
		// ========================================

		template<typename ClassType>
		struct Class {

			ClassType* instance;
			napi_ref   ref;

			static ClassType* Constructor(const napi_context_t& ctx, const std::list<constructor_t*>& ctors)
			{
				//ClassType* inst = nullptr;
				for (std::list<constructor_t*>::const_iterator it = ctors.cbegin();
					it != ctors.cend(); it++)
				{
					constructor_t* self = *it;
					if (self->argc == ctx.argc) {
						return static_cast<ClassType*>(self->invoke(self, ctx));
					}
				}

				// TODO: throw error here

				return nullptr;
			}



			static void Destructor(napi_env env, void* obj, void* finalize_hint)
			{
				//MetaClass<ClassType>* self = static_cast<MetaClass<ClassType>*>(obj);
				//delete self;
			}

			static napi_value New(napi_env env, napi_callback_info info)
			{
				NodeJS::Scope scope = NodeJS::Scope(env);
				napi_status status;

				napi_value target;
				status = napi_get_new_target(env, info, &target);
				assert(status == napi_ok);
				bool is_constructor = target != nullptr;

				napi_context_t ctx;
				ctx.env = env;
				ClassType* externalObj = nullptr;

				napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
				if (ctx.argc > 0)
				{
					ctx.argv = (napi_value*)alloca(sizeof(napi_value)*ctx.argc);
					napi_get_cb_info(env, info, &ctx.argc, ctx.argv, nullptr, nullptr);

					if (ctx.argc == 1) {
						napi_valuetype type;
						napi_typeof(env, ctx.argv[0],&type);
						if (type == napi_external) {
							napi_get_value_external(env, ctx.argv[0], (void**)&externalObj);
						}
					}
				}

				

				class_t* prototype = static_cast<class_t*>(ctx.data);
				Class<ClassType>* obj = new Class<ClassType>();
				if (externalObj) {
					obj->instance = externalObj;
				}
				else {
					obj->instance = Constructor(ctx, prototype->ctors);
				}
			
				napi_wrap(env, ctx.js, obj, &Destructor, nullptr, &obj->ref);
			
				return ctx.js;
			}

			template<typename T>
			static napi_value New_(T&& val) {

				napi_env env = __NAPI_ENV__;
				napi_value argv;
				napi_value ctor = nullptr;
				napi_status status;

				TYPEID Id = TypeID<ClassType>::get();
				module_t& m = Module::module_;
				class_t* proto = m.classes[Id];

				ClassType* obj = new ClassType(val);

				status = napi_get_reference_value(env, proto->ref, &ctor);

				napi_create_external(env, obj, nullptr, nullptr, &argv);

				napi_value instance;
				status = napi_new_instance(env, ctor, 1, &argv, &instance);
				assert(status == napi_ok);

				return instance;

			}

		};


	}
}
