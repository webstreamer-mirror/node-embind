
#ifndef _NODE_EMBIND_EMSCRIPTEN_WIRE_H_
#define _NODE_EMBIND_EMSCRIPTEN_WIRE_H_

#include <memory.h>
#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <string>

#define EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES 0

#define EMSCRIPTEN_ALWAYS_INLINE inline

namespace emscripten {
#ifndef EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES
#define EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES 1
#endif


#if EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES
	constexpr bool has_unbound_type_names = true;
#else
	constexpr bool has_unbound_type_names = false;
#endif

	namespace internal {
		typedef const void* TYPEID;

		// We don't need the full std::type_info implementation.  We
		// just need a unique identifier per type and polymorphic type
		// identification.

		template<typename T>
		struct CanonicalizedID {
			static char c;
			static constexpr TYPEID get() {
				return &c;
			}
		};

		template<typename T>
		char CanonicalizedID<T>::c;

		template<typename T>
		struct Canonicalized {
			typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
		};

		template<typename T>
		struct LightTypeID {
			static /*constexpr*/ TYPEID get() {
				typedef typename Canonicalized<T>::type C;
				if (has_unbound_type_names || std::is_polymorphic<C>::value) {

					static_assert(!has_unbound_type_names,
						"Unbound type names are illegal with RTTI disabled. "
						"Either add -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 to or remove -fno-rtti "
						"from the compiler arguments");
					static_assert(!std::is_polymorphic<C>::value,
						"Canonicalized<T>::type being polymorphic is illegal with RTTI disabled");
				}

				return CanonicalizedID<C>::get();
			}
		};

		template<typename T>
		constexpr TYPEID getLightTypeID(const T& value) {
			typedef typename Canonicalized<T>::type C;
			if (has_unbound_type_names || std::is_polymorphic<C>::value) {

				static_assert(!has_unbound_type_names,
					"Unbound type names are illegal with RTTI disabled. "
					"Either add -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 to or remove -fno-rtti "
					"from the compiler arguments");
				static_assert(!std::is_polymorphic<C>::value,
					"Canonicalized<T>::type being polymorphic is illegal with RTTI disabled");
			}
			return LightTypeID<T>::get();
		}

		template<typename T>
		struct TypeID {
			static constexpr TYPEID get() {
				return LightTypeID<T>::get();
			}
		};

		template<typename T>
		struct TypeID<std::unique_ptr<T>> {
			static constexpr TYPEID get() {
				return TypeID<T>::get();
			}
		};

		template<typename T>
		struct TypeID<T*> {
			static_assert(!std::is_pointer<T*>::value, "Implicitly binding raw pointers is illegal.  Specify allow_raw_pointer<arg<?>>");
		};

		template<typename T>
		struct AllowedRawPointer {
		};

		template<typename T>
		struct TypeID<AllowedRawPointer<T>> {
			static constexpr TYPEID get() {
				return LightTypeID<T*>::get();
			}
		};

		// ExecutePolicies<>

		template<typename... Policies>
		struct ExecutePolicies;

		template<>
		struct ExecutePolicies<> {
			template<typename T, int Index>
			struct With {
				typedef T type;
			};
		};

		template<typename Policy, typename... Remaining>
		struct ExecutePolicies<Policy, Remaining...> {
			template<typename T, int Index>
			struct With {
				typedef typename Policy::template Transform<
					typename ExecutePolicies<Remaining...>::template With<T, Index>::type,
					Index
				>::type type;
			};
		};

		// TypeList<>

		template<typename...>
		struct TypeList {};

		// Cons :: T, TypeList<types...> -> Cons<T, types...>

		template<typename First, typename TypeList>
		struct Cons;

		template<typename First, typename... Rest>
		struct Cons<First, TypeList<Rest...>> {
			typedef TypeList<First, Rest...> type;
		};

		// Apply :: T, TypeList<types...> -> T<types...>

		template<template<typename...> class Output, typename TypeList>
		struct Apply;

		template<template<typename...> class Output, typename... Types>
		struct Apply<Output, TypeList<Types...>> {
			typedef Output<Types...> type;
		};

		// MapWithIndex_

		template<template<size_t, typename> class Mapper, size_t CurrentIndex, typename... Args>
		struct MapWithIndex_;

		template<template<size_t, typename> class Mapper, size_t CurrentIndex, typename First, typename... Rest>
		struct MapWithIndex_<Mapper, CurrentIndex, First, Rest...> {
			typedef typename Cons<
				typename Mapper<CurrentIndex, First>::type,
				typename MapWithIndex_<Mapper, CurrentIndex + 1, Rest...>::type
			>::type type;
		};

		template<template<size_t, typename> class Mapper, size_t CurrentIndex>
		struct MapWithIndex_<Mapper, CurrentIndex> {
			typedef TypeList<> type;
		};

		template<template<typename...> class Output, template<size_t, typename> class Mapper, typename... Args>
		struct MapWithIndex {
			typedef typename internal::Apply<
				Output,
				typename MapWithIndex_<Mapper, 0, Args...>::type
			>::type type;
		};


		template<typename ArgList>
		struct ArgArrayGetter;

		template<typename... Args>
		struct ArgArrayGetter<TypeList<Args...>> {
			static const TYPEID* get() {
				static /*constexpr*/ TYPEID types[] = { TypeID<Args>::get()... };
				return types;
			}
		};

		// WithPolicies<...>::ArgTypeList<...>

		template<typename... Policies>
		struct WithPolicies {
			template<size_t Index, typename T>
			struct MapWithPolicies {
				typedef typename ExecutePolicies<Policies...>::template With<T, Index>::type type;
			};

			template<typename... Args>
			struct ArgTypeList {
				unsigned getCount() const {
					return sizeof...(Args);
				}

				const TYPEID* getTypes() const {
					return ArgArrayGetter<
						typename MapWithIndex<TypeList, MapWithPolicies, Args...>::type
					>::get();
				}
			};
		};

		template<typename ElementType>
		struct memory_view {
			memory_view() = delete;
			explicit memory_view(size_t size, const ElementType* data)
				: size(size)
				, data(data)
			{}

			const size_t size; // in elements, not bytes
			const void* const data;
		};

		// Note that 'data' is marked const just so it can accept both
		// const and nonconst pointers.  It is certainly possible for
		// JavaScript to modify the C heap through the typed array given,
		// as it merely aliases the C heap.
		template<typename T>
		inline memory_view<T> typed_memory_view(size_t size, const T* data) {
			static_assert(internal::typeSupportsMemoryView<T>(),
				"type of typed_memory_view is invalid");
			return memory_view<T>(size, data);
		}

	}

}

#endif // !_NODE_EMBIND_EMSCRIPTEN_WIRE_H_
