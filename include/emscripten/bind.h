
#ifndef _NODE_EMBIND_EMSCRIPTEN_BIND_H_
#define _NODE_EMBIND_EMSCRIPTEN_BIND_H_

// EMBIND_STD_STRING_IS_UTF8

#include <stddef.h>
#include <assert.h>
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <type_traits>
#include <stdio.h>

#include <tuple>

#define EMSCRIPTEN_NODE_EMBIND

#include <emscripten/wire.h>

namespace emscripten {

	namespace internal {
		typedef long GenericEnumValue;
		typedef void* GenericFunction;
		typedef void(*VoidFunctionPtr)(void);
	}
}

#include <emscripten/n-api/n-napi.h>

namespace emscripten {
	////////////////////////////////////////////////////////////////////////////////
	// POLICIES
	////////////////////////////////////////////////////////////////////////////////

	template<int Index>
	struct arg {
		static constexpr int index = Index + 1;
	};

	struct ret_val {
		static constexpr int index = 0;
	};



	// whitelist all raw pointers
	struct allow_raw_pointers {
		template<typename InputType, int Index>
		struct Transform {
			typedef typename std::conditional<
				std::is_pointer<InputType>::value,
				internal::AllowedRawPointer<typename std::remove_pointer<InputType>::type>,
				InputType
			>::type type;
		};
	};

	// this is temporary until arg policies are reworked
	template<typename Slot>
	struct allow_raw_pointer : public allow_raw_pointers {
	};

	////////////////////////////////////////////////////////////////////////////////
	// select_overload and select_const
	////////////////////////////////////////////////////////////////////////////////

	template<typename Signature>
	Signature* select_overload(Signature* fn) {
		return fn;
	}

	template<typename Signature, typename ClassType>
	auto select_overload(Signature(ClassType::*fn)) -> decltype(fn) {
		return fn;
	}

	template<typename ClassType, typename ReturnType, typename... Args>
	auto select_const(ReturnType(ClassType::*method)(Args...) const) -> decltype(method) {
		return method;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Invoker
	////////////////////////////////////////////////////////////////////////////////

	namespace internal {
		template<typename ReturnType, typename... Args>
		struct Invoker {
			typedef ReturnType(*Fn)(Args...);
			
			typedef napi::Invoker<ReturnType, Args...> I;

			static napi_value invoke(
				const napi::function_t* self, 
				const napi::context_t& ctx)
			{
				if (self->argc == ctx.argc) {
					return napi::value<ReturnType>::napivalue(ctx.env,I::invoke(ctx.env, ctx.argv, (Fn)self->function));
				}

				// recursion to find out override functin
				napi::function_t* fn = self->next;
				while (fn) {
					if (fn->argc == ctx.argc) {
						return fn->invoke(fn, ctx);
					}
				}

				// no match overwrite function match the provide arguments.
				napi_throw_error(ctx.env, NODE_EMBIND_ERROR_ARGC, NODE_EMBIND_ERROR_ARGC_MSG);
				return nullptr;
			}

		};

		template<typename... Args>
		struct Invoker<void, Args...> {

			typedef void(*Fn)(Args...);
			typedef napi::Invoker<void, Args...> I;

			static napi_value invoke(const napi::function_t* self, const napi::context_t& ctx)
			{
				if (self->argc == ctx.argc) {
					I::invoke(ctx.env, ctx.argv, (Fn)self->function);
					return nullptr;
				}

				napi::function_t* fn = self->next;
				while (fn) {
					if (fn->argc == ctx.argc) {
						return fn->invoke(fn, ctx);
					}
				}

				napi_throw_error(ctx.env, NODE_EMBIND_ERROR_ARGC, NODE_EMBIND_ERROR_ARGC_MSG);

				return nullptr;
			}

		};
	}

	////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	////////////////////////////////////////////////////////////////////////////////

	template<typename ReturnType, typename... Args, typename... Policies>
	void function(const char* name, ReturnType(*fn)(Args...), Policies...) {

		using namespace internal;

		auto invoker = &Invoker<ReturnType, Args...>::invoke;

		napi::register_function(
			name,
			sizeof...(Args),
			reinterpret_cast<GenericFunction>(invoker),
			reinterpret_cast<GenericFunction>(fn));

	}



	namespace internal {
		template<typename ClassType, typename... Args>
		ClassType* operator_new(Args&&... args) {
			return new ClassType(std::forward<Args>(args)...);
		}

		template<typename WrapperType, typename ClassType, typename... Args>
		WrapperType wrapped_new(Args&&... args) {
			return WrapperType(new ClassType(std::forward<Args>(args)...));
		}

		//template<typename ClassType, typename... Args>
		//ClassType* raw_constructor(
		//	typename internal::BindingType<Args>::WireType... args
		//) {
		//	return new ClassType(
		//		internal::BindingType<Args>::fromWireType(args)...
		//	);
		//}

		template<typename ClassType>
		void raw_destructor(ClassType* ptr) {
			delete ptr;
		}
	}

    namespace internal {
        struct NoBaseClass {
            template<typename ClassType>
            static void verify() {
            }

            static TYPEID get() {
                return nullptr;
            }

            template<typename ClassType>
            static VoidFunctionPtr getUpcaster() {
                return nullptr;
            }

            template<typename ClassType>
            static VoidFunctionPtr getDowncaster() {
                return nullptr;
            }
        };

        // NOTE: this returns the class type, not the pointer type
        template<typename T>
        inline TYPEID getActualType(T* ptr) {
            return getLightTypeID(*ptr);
        };
    }



	namespace internal {
		template<typename ClassType, typename... Args>
		struct ConstructorInvoker {

			typedef ClassType* (*Fn)(Args...);
			typedef napi::Invoker<ClassType*, Args...> I;
			static void* invoke(const napi::constructor_t* self, const napi::context_t& ctx)
			{
				assert(ctx.argc == sizeof...(Args));
				return I::invoke(ctx.env, ctx.argv, (Fn)self->function);
			}
		};
	}


    template<typename ClassType, typename BaseSpecifier = internal::NoBaseClass>
    class class_ {
    public:
        typedef ClassType class_type;
        typedef BaseSpecifier base_specifier;

        class_() = delete;

        explicit class_(const char* name) {
            using namespace internal;

            BaseSpecifier::template verify<ClassType>();

			auto New = napi::Class<ClassType>::New;
			auto Delete = napi::Class<ClassType>::Delete;

            napi::register_class(
				name,
                TypeID<ClassType>::get(),
                reinterpret_cast<GenericFunction>(New),
				reinterpret_cast<GenericFunction>(Delete)
				);
        }

		template<typename... ConstructorArgs, typename... Policies>
		inline const class_& constructor(Policies... policies) const {
			return constructor(
				&internal::operator_new<ClassType, ConstructorArgs...>,
				policies...);
		}

		template<typename... Args, typename ReturnType, typename... Policies>
		inline const class_& constructor(ReturnType(*factory)(Args...), Policies...) const {
			using namespace internal;

			auto invoke = &ConstructorInvoker<ClassType, Args...>::invoke;
			napi::register_class_constructor(
				TypeID<ClassType>::get(),
				sizeof...(Args),
				reinterpret_cast<GenericFunction>(invoke),
				reinterpret_cast<GenericFunction>(factory));
			return *this;
		}
	};


}

#define EMSCRIPTEN_BINDINGS(name)                                       \
    static struct EmscriptenBindingInitializer_##name {                 \
        EmscriptenBindingInitializer_##name();                          \
    } EmscriptenBindingInitializer_##name##_instance;                   \
    EmscriptenBindingInitializer_##name::EmscriptenBindingInitializer_##name()

#endif // !_NODE_EMBIND_EMSCRIPTEN_BIND_H_