
#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_

#include "common.h"
#include "value/fundamental.h"
#include "value/class.h"

namespace emscripten {
	namespace napi {
		namespace internal {
			template<typename T>
			using remove_decorate = typename ::emscripten::internal::remove_decorate<T>;

			template<typename T>
			using _is_fundamental = typename std::is_fundamental< typename remove_decorate<T>::type > ;

			template<typename T>
			using _is_class = typename std::is_class< typename remove_decorate<T>::type > ;

			template<typename T, bool fundamental, bool klass>
			struct _Adapter
			{};

			template<typename T>
			struct _Adapter<T,true,false>
			{
				using type = typename Fundamental<T>;
			};

			template<typename T>
			struct _Adapter<T, false, true>
			{
				using type = typename Class<T>;
			};

			template<typename T>
			using Caster = typename _Adapter<T, _is_fundamental<T>::value, _is_class<T>::value>::type;
			//
			//template<typename T>
			//struct _fundamental {
			//	using type = typename std::conditional < _is_fundamental<T>::value, Fundamental<T>, T>::type;
			//};
			//
			//template<typename T>
			//struct _class {
			//	using type = typename std::conditional < _is_class<T>::value, 
			//		Class<T>, typename _fundamental<T>::type >::type;
			//};
			//
			//
			//template<typename T>
			//struct _selector {
			//	using type = typename _class<T>::type;				
			//};

			template<typename T>
			struct Value : public Caster<T>
			{
				//using type = typename _selector<T>::type;
				using _Base = typename Caster<T>;
				Value(napi_env env)
					: _Base(env)
				{}
			};
		}

		template <typename T>
		using value = ::emscripten::napi::internal::Value<T>;
	}
}


#endif //!_NODE_EMBIND_EMSCRIPTEN_NAPI_VALUE_H_