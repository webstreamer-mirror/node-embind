

#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_



#include "class.h"
#include "enum.h"
#include "constant.h"

namespace emscripten {
	namespace internal {

		struct class_t;
		struct function_t;
		struct module_t {
			std::map<TYPEID, class_t*> classes;
			std::map<TYPEID, enum_t*> enums;
			std::list<constant_t*> constants;
			std::list<function_t*> functions;
			napi_value env_;
		};

		class Module
		{
		public:
			static napi_value Init(napi_env env, napi_value exports) {

				std::map<TYPEID, class_t*>& classes = module_.classes;
				std::map<TYPEID, enum_t*>& enums = module_.enums;
				std::list<constant_t*>& constants = module_.constants;
				Preprocess(module_.functions);

				auto prop = make_napi_property_table(module_.functions);

				for (std::list<constant_t*>::iterator it = constants.begin();
					it != constants.end(); it++) {
					Register(prop, *it, env, exports);
				}

				napi_define_properties(env, exports, prop.size(), prop.data());

				for (std::map<TYPEID, class_t*>::iterator it = classes.begin();
					it != classes.end(); it++) {
					//class_t* c = it->second;

					Initialize(it->second, env, exports);

				}

				for (std::map<TYPEID, enum_t*>::iterator it = enums.begin();
					it != enums.end(); it++) {
					//class_t* c = it->second;

					Register(it->second, env, exports);
				}

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
		//		static napi_value get_env(napi_value e = nullptr) { return Module::env(e); }
	}
}

#define NODE_EMBIND_EMSCRIPTEN_MODULE_INIT           \
     namespace emscripten { namespace internal {     \
         module_t Module::module_;                   \
     } }                                             \
     NAPI_MODULE(NODE_GYP_MODULE_NAME, emscripten::internal::Module::Init);  \


#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_MODULE_H_