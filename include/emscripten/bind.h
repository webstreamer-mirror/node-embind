
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
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <stdio.h>


#include <tuple>

namespace emscripten {

	namespace internal {
		typedef long GenericEnumValue;
		typedef void* GenericFunction;
		typedef void(*VoidFunctionPtr)(void);
	}
}

#include "n-api/n-napi.h"

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

			static napi_value invoke(const function_t* self, const napi_context_t& ctx)
			{
				if (self->argc == ctx.argc) {
					return napi::value<ReturnType>(ctx.env).napi_cast(
						I::invoke(ctx.env, ctx.argv, (Fn)self->function/*static_cast<Fn>(self->function)*/)
					);
				}

				function_t* fn = self->next;
				while (fn) {
					if (fn->argc == ctx.argc) {
						return fn->invoke(fn, ctx);
					}
				}

				// TODO: throw error here

				return nullptr;
			}

		};

		template<typename... Args>
		struct Invoker<void, Args...> {

			typedef void(*Fn)(Args...);
			typedef napi::Invoker<void, Args...> I;

			static napi_value invoke(const function_t* self, const napi_context_t& ctx)
			{
				if (self->argc == sizeof...(Args)) {
					I::invoke(ctx.env, ctx.argv, (Fn)self->function/*static_cast<Fn>(self->function)*/);
					return nullptr;
				}

				function_t* fn = self->next;
				while (fn) {
					if (fn->argc == sizeof...(Args)) {
						return fn->invoke(fn, ctx);
					}
				}

				// TODO: throw error here

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
		typename WithPolicies<Policies...>::template ArgTypeList<ReturnType, Args...> args;
		auto invoker = &Invoker<ReturnType, Args...>::invoke;

		_embind::register_function(
			name,
			args.getCount(),
			args.getTypes(),
			getSignature(invoker),
			reinterpret_cast<GenericFunction>(invoker),
			reinterpret_cast<GenericFunction>(fn));

	}

}

/*
    You must past below code to your .cpp file to initialize

#ifdef NODE_EMBIND_EMSCRIPTEN_INITIALIZE
    NODE_EMBIND_EMSCRIPTEN_INITIALIZE
#endif

*/
#define NODE_EMBIND_EMSCRIPTEN_INITIALIZE \
    NODE_EMBIND_EMSCRIPTEN_MODULE_INIT \
    NODE_EMBIND_EMSCRIPTEN_COMMON_INIT

#define EMSCRIPTEN_BINDINGS(name)                                       \
    static struct EmscriptenBindingInitializer_##name {                 \
        EmscriptenBindingInitializer_##name();                          \
    } EmscriptenBindingInitializer_##name##_instance;                   \
    EmscriptenBindingInitializer_##name::EmscriptenBindingInitializer_##name()

#include "n-api/_embind.inl"

#endif // !_NODE_EMBIND_EMSCRIPTEN_BIND_H_