
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_ENUM_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_ENUM_H_

#include "common.h"
#include "function.h"
#include "property.h"

namespace emscripten {
	namespace internal {


		struct enum_t
		{
			typedef long type;

			enum_t() :name(nullptr), New(nullptr),ref(nullptr)
			{}
			const char* name;
			std::map<const char*, type> items;

			napi_ref ref;
			napi_value(*New)(napi_env env, napi_callback_info info);
		};





		

		// ========================================
		//   Enum
		// ========================================

		template<typename EnumType>
		struct Enum {

			EnumType value;
			napi_ref ref;

			static enum_t* prototype;


			static void Destructor(napi_env env, void* obj, void* finalize_hint)
			{
				//MetaClass<ClassType>* self = static_cast<MetaClass<ClassType>*>(obj);
				//delete self;
			}
			
			static napi_value New(napi_env env, napi_callback_info info)
			{
				NodeJS::Scope scope = NodeJS::Scope(env);
				return nullptr;
			
                //size_t argc=1;
                //napi_value argv;
                //napi_value js;
                //void* data;
				//napi_get_cb_info(env, info, &argc, &argv, &js, &data);
				//enum_t* proto = (enum_t*)data;
				//
				//if (argc != 1) {
				//	return nullptr;
				//}
				//
 				//napi_valuetype type;
				//napi_typeof(env, argv, &type);
				//if (type != napi_number) {
				//	return nullptr;
				//}
				//
				////int32_t result;
				////napi_get_value_int32(env, argv, &result);
				////
				////
				//////typedef std::map<enum_t::type, enum_t::content_t*> Item;
				//////Item& items = proto->items;
				//////Item::iterator it = items.find((enum_t::type)result);
				////
				////Enum<EnumType>* obj = new Enum<EnumType>();
				////obj->value = (EnumType)result;
				//
				//napi_wrap(env, js, obj, &Destructor, nullptr, &obj->ref);
				////__values[obj->value] = js;
				//
				//return js;
			}
			

			//inline static napi_value napi_getter(napi_env env, napi_callback_info info) {
			//	napi_context_t ctx;
			//	ctx.env = env;
			//	ctx.argv = nullptr;
			//	auto scope = NodeJS::Scope(env);
			//	
			//	napi_get_cb_info(env, info, &ctx.argc, nullptr, &ctx.js, &ctx.data);
			//
			//	enum_t::content_t* c = static_cast<enum_t::content_t*>(ctx.data);
			//	enum_t* proto = c->proto;
			//	if (c->value==nullptr) {
			//		size_t argc = 1;
			//		napi_value argv, ctor;
			//		napi_create_int32(env, (int32_t)c->type, &argv);
			//
			//		napi_get_reference_value(env, proto->ref, &ctor);
			//
			//		napi_new_instance(env, ctor, argc, &argv, &c->value);
			//	}
			//	std::cout <<" * " << c << " @ " << c->names.back() << " : " << c->value << std::endl;
			//	return c->value;
			//}

		};


		inline void Register(enum_t* prototype, napi_env env, napi_value exports)
		{
			std::vector<napi_property_descriptor> prop;
			napi_value ctor = nullptr;
			typedef  std::map<const char*, enum_t::type> Item;
			Item& items = prototype->items;

			for (Item::const_iterator item = items.cbegin(); item != items.cend(); item++) {

				enum_t::type v = item->second;
				napi_value value;
				if (sizeof(enum_t::type) == sizeof(int32_t)) {
					napi_create_int32(env, (int32_t)item->second, &value);
				}
				else {
					napi_create_int64(env, (int64_t)item->second, &value);
				}

				napi_property_descriptor desc = {
					item->first,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					value, napi_static,nullptr
				};
				prop.push_back(desc);

			}

			napi_define_class(env, prototype->name, -1, prototype->New, prototype, prop.size(), prop.data(), &ctor);
			napi_create_reference(env, ctor, 1, &(prototype->ref));
			napi_set_named_property(env, exports, prototype->name, ctor);
		}

	}
}


#endif //!_NODE_EMBIND_EMSCRIPTEN_NAPI_ENUM_H_