#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_

#include <emscripten/n-api/function.h>

NS_NAPI_BEGIN

struct function_t;
struct module_t {
	std::list<function_t*> functions;
	napi_value env_;
};

class Module
{
public:
	static napi_value Init(napi_env env, napi_value exports) {

		Preprocess(module_.functions);

		auto prop = make_napi_property_table(module_.functions);

		napi_define_properties(env, exports, prop.size(), prop.data());



		return exports;
	}

	static module_t module_;
	static napi_value env(napi_value e = nullptr) {
		if (e) module_.env_ = e;
		return module_.env_;
	}
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