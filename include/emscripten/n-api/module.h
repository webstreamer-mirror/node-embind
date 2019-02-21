#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_

#include <emscripten/n-api/function.h>

NS_NAPI_BEGIN

struct function_t;
struct module_t {
	std::list<function_t*> functions;
	napi_value env_;
};

inline module_t& node_module()
{
	static module_t module;
	return module;
}

class Module
{
public:

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

	static napi_value Init(napi_env env, napi_value exports) {
		module_t& m = node_module();

		Preprocess(m.functions);

		auto prop = make_napi_property_table(m.functions);
		
		prop.push_back({ "EMBIND_STD_STRING_IS_UTF8", nullptr,nullptr,
			EMBIND_STD_STRING_IS_UTF8_getter, EMBIND_STD_STRING_IS_UTF8_setter,
			nullptr,napi_default,nullptr });

		napi_define_properties(env, exports, prop.size(), prop.data());



		return exports;
	}

	static	bool EMBIND_STD_STRING_IS_UTF8;
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
	unsigned argCount,
	const TYPEID argTypes[],
	GenericFunction invoker,
	GenericFunction function);


NS_NAPI_END


#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_