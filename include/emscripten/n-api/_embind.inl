
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_EMBIND_INL_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_EMBIND_INL_

#include "invoker.h"
#include "class.h"
#include "enum.h"
#include "module.h"

namespace emscripten {
	namespace internal {
		namespace napi {
			inline void _embind::register_function(
				const char* name,
				unsigned argCount,
				const TYPEID argTypes[],
				GenericFunction invoker,
				GenericFunction function)
			{
				module_t& m = Module::module_;
				function_t* f = new function_t();
				f->name = name;
				f->argc = argCount - 1;
				typedef napi_value(*Fn)(const function_t*, const context_t&);
#if _MSVC_VER
				f->invoke = static_cast<napi_value(*)(const function_t*, const context_t&)>(invoker);
#else
				f->invoke = (Fn)invoker;
#endif
				f->function = function;
				m.functions.push_back(f);
			}



			inline void _embind::register_class(
				const char* name,
				TYPEID classType,
				TYPEID pointerType,
				TYPEID constPointerType,
				GenericFunction New)
			{

				module_t& m = Module::module_;
				class_t* prototype = new class_t();
				prototype->name = name;

				typedef napi_value(*Fn)(napi_env, napi_callback_info);
				prototype->New = (Fn)(New);
				m.classes[classType] = prototype;

			}

			inline void _embind::register_class_constructor(
				TYPEID classType,
				unsigned argCount,
				const TYPEID argTypes[],
				const char* invokerSignature,
				GenericFunction invoker,
				GenericFunction function)
			{
				module_t& m = Module::module_;
				constructor_t* prototype = new constructor_t();
				prototype->argc = argCount - 1;

				typedef class_ptr(*Fn) (const constructor_t*, const context_t&);
				prototype->invoke = (Fn)(invoker);
				prototype->function = function;

				m.classes[classType]->ctors.push_back(prototype);
			}

			inline void _embind::register_class_property(
				TYPEID classType,
				const char* fieldName,
				GenericFunction getter,
				void* getterContext,
				GenericFunction setter,
				void* setterContext)
			{
				module_t& m = Module::module_;
				property_t* prop = new property_t;
				prop->name = fieldName;

				typedef napi_value(*Setter)(const property_t*, const context_t&);
				prop->setter = (Setter)setter;
				prop->setter_context = setterContext;

				typedef napi_value(*Getter)(const property_t*, const context_t&);

				prop->getter = (Getter)getter;
				prop->getter_context = getterContext;
				m.classes[classType]->property.push_back(prop);
			}

			inline void _embind::register_class_class_property(
				TYPEID classType,
				const char* fieldName,
				const void* fieldContext,
				GenericFunction getter,
				GenericFunction setter)
			{
				module_t& m = Module::module_;
				property_t* prop = new property_t;
				prop->name = fieldName;

				typedef napi_value(*Setter)(const property_t*, const context_t&);
				prop->setter = (Setter)setter;
				prop->setter_context = (void*)fieldContext;

				typedef napi_value(*Getter)(const property_t*, const context_t&);

				prop->getter = (Getter)getter;
				prop->getter_context = (void*)fieldContext;
				prop->attributes = napi_static;
				m.classes[classType]->property.push_back(prop);
			}
			inline void _embind::register_class_function(
				TYPEID classType,
				const char* methodName,
				unsigned argCount,
				const TYPEID argTypes[],
				GenericFunction invoker,
				void* context,
				unsigned isPureVirtual)
			{
				module_t& m = Module::module_;
				function_t* fn = new function_t();
				fn->argc = argCount - 1;
				fn->name = methodName;
				typedef napi_value(*Fn)(const function_t*, const context_t&);
				fn->invoke = (Fn)(invoker);
				fn->function = context;
				m.classes[classType]->function.push_back(fn);
			}

			inline void _embind::register_class_class_function(
				TYPEID classType,
				const char* methodName,
				unsigned argCount,
				const TYPEID argTypes[],
				const char* invokerSignature,
				GenericFunction invoker,
				GenericFunction method)
			{
				module_t& m = Module::module_;
				function_t* fn = new function_t();
				fn->argc = argCount - 1;
				fn->name = methodName;

				typedef napi_value(*Fn)(const function_t*, const context_t&);
				fn->invoke = (Fn)invoker;
				fn->function = method;
				fn->attributes = napi_static;
				m.classes[classType]->function.push_back(fn);
			}

			inline void _embind::register_enum(
				TYPEID enumType,
				const char* name,
				size_t size,
				bool isSigned,
				GenericFunction New,
				napi_callback getter) {

				module_t& m = Module::module_;
				enum_t* prototype = new enum_t();
				prototype->name = name;
				//			prototype->getter = getter;
				typedef napi_value(*Fn)(napi_env, napi_callback_info);
				prototype->New = (Fn)New;

				m.enums[enumType] = prototype;

			}

			inline void _embind::register_enum_value(
				TYPEID enumType,
				const char* valueName,
				GenericEnumValue value)
			{
				module_t& m = Module::module_;
				enum_t* prototype = new enum_t();
				enum_t* e = m.enums[enumType];
				e->items[valueName] = value;
				//typedef std::map<enum_t::type, enum_t::content_t*> Item;
				//Item& items = e->items;
				//enum_t::type type = (enum_t::type)value;
				//
				//Item::iterator it = items.find(type);
				//if (it == items.end()) {
				//	Item item;
				//	enum_t::content_t* c = new enum_t::content_t(e,type);
				//	c->names.push_back(valueName);
				//	items[type] = c;
				//}
				//else {
				//	it->second->names.push_back(valueName);
				//}
			}

			inline void _embind::register_constant(
				const char* name,
				TYPEID constantType,
				void*  function,
				void*  const_value)
			{
				module_t& m = Module::module_;
				constant_t* c = new constant_t();
				c->name = name;
				c->constant_value = const_value;
				c->function = function;
				m.constants.push_back(c);

			}

		}
	}
}
#endif //!_NODE_EMBIND_EMSCRIPTEN_INTERNAL_EMBIND_IMPLEMENT_H_