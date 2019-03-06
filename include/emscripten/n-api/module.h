#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_

#include <emscripten/n-api/function.h>
#include <emscripten/n-api/class.h>
NS_NAPI_BEGIN

struct function_t;
struct module_t {
	napi_value env_;

	std::list<function_t*> functions;
	std::map<TYPEID, class_t*> classes;
};

inline module_t& node_module()
{
	static module_t module;
	return module;
}

class Module
{
public:
	static	bool EMBIND_STD_STRING_IS_UTF8;
	static  bool GARBAGE_CLEANING;

	static napi_value EMBIND_STD_STRING_IS_UTF8_setter(napi_env env, napi_callback_info info) {
		size_t argc = 1;
		napi_value argv = nullptr;
		napi_get_cb_info(env, info, &argc, &argv, nullptr, nullptr);
		if (argc == 1) {
			napi_get_value_bool(env, argv, &EMBIND_STD_STRING_IS_UTF8);
		}
		return nullptr;
	}

	static napi_value EMBIND_STD_STRING_IS_UTF8_getter(napi_env env, napi_callback_info info) {
		napi_value value=nullptr;
		napi_get_boolean(env, EMBIND_STD_STRING_IS_UTF8, &value);
		return value;
	}
	static napi_value Init(napi_env env, napi_value exports);



protected:

	static void Preprocess(std::list<function_t*>& functions) {
		// merge the overload function
		std::map<const char*, function_t*> card;
		std::list<function_t*>& fns = functions;
		for (std::list<function_t*>::iterator it = fns.begin(); it != fns.end();) {
			const char* name = (*it)->name;
			std::map<const char*, function_t*>::iterator c = card.find(name);
			if (c == card.end()) {
				card[name] = *it;
				it++;
			}
			else {
				c->second->next = *it;
				fns.erase(it++);
			}
		}
	}

	static std::vector<napi_property_descriptor> make_napi_property_table(std::list<function_t*>& functions)
	{

		std::vector<napi_property_descriptor> prop;

		for (std::list<function_t*>::iterator it = functions.begin(); it != functions.end(); it++) {
			function_t* fn = *it;
			prop.push_back(
				{ fn->name,nullptr,
				napi_method,
				nullptr, nullptr, nullptr, napi_default, (void*)fn }
			);
		}
		return prop;
	}
};

// ==================================
//   Registers
// ==================================

void register_function(
	const char* name,
	unsigned argc,
	GenericFunction invoker,
	GenericFunction function);

void register_class(
	const char* name,
	TYPEID classType,
	GenericFunction New,
	GenericFunction Delete,
    class_t** pprototype,
    class_t::SubType subtype=class_t::generic);	

void register_class_constructor(
	TYPEID classType,
	unsigned argc,
	GenericFunction invoker,
	GenericFunction function);

void register_class_property(
	TYPEID classType,
	const char* fieldName,
	GenericFunction getter,
	void* getterContext,
	GenericFunction setter,
	void* setterContext);

void register_class_function(
    TYPEID classType,
    const char* methodName,
    unsigned argCount,
    GenericFunction invoker,
    void* context,
    unsigned isPureVirtual);

void register_class_class_function(
    TYPEID classType,
    const char* methodName,
    unsigned argc,
    GenericFunction invoker,
    GenericFunction method);

void register_class_class_property(
    TYPEID classType,
    const char* fieldName,
    const void* fieldContext,
    GenericFunction getter,
    GenericFunction setter);

NS_NAPI_END


#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_