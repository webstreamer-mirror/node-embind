#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CLASS_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CLASS_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/property.h>

NS_NAPI_BEGIN


	struct constructor_t {
		size_t argc;
		void* function; // C++ function
		void* (*invoke)(const constructor_t*, const context_t&);

		constructor_t()
			:argc(0), function(nullptr), invoke(nullptr)
		{}
	};


	struct class_t
	{
		class_t() :name(nullptr), New(nullptr), ctor(nullptr),ref(nullptr)
		{}
		const char* name;  
		napi_callback New; // C++ function to create class instance
		napi_callback Delete; // C++ function to create class instance
		napi_value ctor;   // constructor function for the class.
		napi_ref ref;      // reference use to hold ctor

		std::list<constructor_t*> ctors;
		std::list<function_t*> function;
		std::list<property_t*> property;
	};


    // ========================================
    //   Class
    // ========================================
    struct IClass {
        class_t* prototype;
        void*    instance;
        napi_ref ref;

        IClass(): prototype(nullptr),instance(nullptr),ref(nullptr)      
        {}

	};

    template<typename ClassType>
    struct Class //: public IClass 
	{
		class_t* prototype;
		ClassType*    instance;
		napi_ref ref;

		Class() : prototype(nullptr), instance(nullptr), ref(nullptr)
		{}

        static ClassType* Constructor(const context_t& ctx, const std::list<constructor_t*>& ctors)
        {
            for (std::list<constructor_t*>::const_iterator it = ctors.cbegin();
                it != ctors.cend(); it++)
            {
                constructor_t* self = *it;
                if (self->argc == ctx.argc) {
                    return static_cast<ClassType*>(self->invoke(self, ctx));
                }
            }
            // no constructor match the provide arguments.
            napi_throw_error(ctx.env, NODE_EMBIND_ERROR_ARGC, NODE_EMBIND_ERROR_CTOR_ARGC_MSG);
            return nullptr;
        }

        static void Destructor(napi_env env, void* obj, void* finalize_hint)
        {

            Class<ClassType>* self = static_cast<Class<ClassType>*>(obj);
			if (self->instance) {
				delete self->instance;
			}
			
			if (self->ref) {
				napi_delete_reference(env, self->ref);
			}

			if (Module::GARBAGE_CLEANING) {
				memset(self, 0, sizeof(Class<ClassType>));
			}
			delete self;
        }

		static napi_value Delete(napi_env env, napi_callback_info info)
		{
			context_t ctx;
			ctx.env = env;
			ctx.argc = 0;
			napi_value js=nullptr;
			napi_get_cb_info(env, info, &ctx.argc, nullptr, &js, nullptr);
			assert(ctx.argc == 0);

			Class<ClassType>* self = nullptr;
			napi_unwrap(env, js, (void**)&self);

			if (self->instance) {
				delete self->instance;
				self->instance = nullptr;
			}

			if (self->ref) {
				napi_delete_reference(env, self->ref);
				self->ref = nullptr;
			}
			return nullptr;

		}

        static napi_value New(napi_env env, napi_callback_info info)
        {
            context_t ctx;
            ctx.env  = env;
            ctx.argc = 0;
            ctx.argv = nullptr;

            napi_value js=nullptr; // this in js
			class_t* prototype = nullptr;
            
            napi_get_cb_info(env, info, &ctx.argc, nullptr, &js, (void**)&prototype);
            if (ctx.argc > 0)
            {
                ctx.argv = (napi_value*)alloca(sizeof(napi_value)*ctx.argc);
                napi_get_cb_info(env, info, &ctx.argc, ctx.argv, nullptr, nullptr);
            }
			
			std::list<constructor_t*>& ctors = prototype->ctors;

			Class<ClassType>* self = new Class<ClassType>();
			self->prototype = prototype;
			self->instance = Constructor(ctx, ctors);

            assert(self->instance);
        
            napi_wrap(env, js, self, &Destructor, self, &self->ref);
        
            return js;
        }

    };

    static void Register(class_t* prototype, napi_env env, napi_value exports)
    {
        std::vector<napi_property_descriptor> prop;

		prop.push_back({ "delete",nullptr,prototype->Delete,nullptr,nullptr,nullptr,napi_default,nullptr });
		for (std::list<property_t*>::iterator it = prototype->property.begin();
			it != prototype->property.end(); it++) {
			const char* name = (*it)->name;			
			napi_callback getter = (*it)->getter ? &napi_getter : nullptr;
			napi_callback setter = (*it)->setter ? &napi_setter : nullptr;

			prop.push_back({name,nullptr,nullptr,getter,setter,	0,napi_default,*it});
		}


        napi_define_class(env, prototype->name, NAPI_AUTO_LENGTH , 
			prototype->New, prototype,
            prop.size(), prop.data(), 
			&prototype->ctor);

        napi_create_reference(env, prototype->ctor, 1, &prototype->ref);
        napi_set_named_property(env, exports, prototype->name, prototype->ctor);
    }




NS_NAPI_END

#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_CLASS_H_
