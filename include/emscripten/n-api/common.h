#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_


#include <memory>
#include <malloc.h>

#include <list>
#include <map>
#include <vector>

#include <node_api.h>

namespace emscripten {
	namespace internal {
		template<typename T>
		struct remove_decorate {
			typedef typename std::remove_cv<T>::type _cv;

			typedef typename std::remove_pointer<_cv>::type   _ptr;
			typedef typename std::remove_reference<_cv>::type _ref;

			typedef typename std::remove_reference<_ptr>::type type;
		};

		template< class T >
		using remove_decorate_t = typename remove_decorate<T>::type;

		// Instance cast
		template<class T>
		struct Instance {
			static T cast(void* p) { return * (T*)p; }
		};

		template<class T>
		struct Instance<const T> {
			static const T cast(void* p) { return *(T*)p; }
		};

		template<class T>
		struct Instance<T&> {
			static T& cast(void* p) { return *(T*)p; }
		};

		template<class T>
		struct Instance<const T&> {
			static const T& cast(void* p) { return *(T*)p; }
		};

		template<class T>
		struct Instance<T&&> {
			static T&& cast(void* p) { return *(T*)p; }
		};


		// @@
		//template<class T>
		//struct _Instance {
		//	typedef T type;
		//	static T cast(T* p) { return *p; }
		//};
		//
		//template<class T>
		//struct _Instance<const T> {
		//	typedef T type;
		//	static const T cast(T* p) { return *p; }
		//};
		//
		//template<class T>
		//struct _Instance<T&> {
		//	typedef T type;
		//	static T& cast(T* p) { return *p; }
		//};
		//
		//template<class T>
		//struct _Instance<const T&> {
		//	typedef T type;
		//	static const T& cast(T* p) { return *p; }
		//};
		//
		//template<class T>
		//struct _Instance<T&&> {
		//	typedef T type;
		//	static T&& cast(T* p) { return *p; }
		//};
		//
		//template<typename T>
		//T instance_cast(typename _Instance<T>::type* p) {
		//	return _Instance<T>::cast(p);
		//}






		typedef void* class_ptr;

		struct napi_context_t {
			napi_context_t()
				: data(nullptr),env(nullptr),argc(0),argv(nullptr),js(nullptr)
			{}
			void* data; // function_t property_t ....
			napi_env env;
			size_t argc;
			napi_value* argv;
			napi_value js;
		};

		typedef napi_value (*InvokerFunction)( const void* self, const napi_context_t&);
		typedef class_ptr  (*ConstructorFunction)(const void* self, const napi_context_t&);


		/////////////////////////////////////////
		struct NodeJS {

			static napi_env env_;
			static napi_env env() {
				return NodeJS::env_;
			}

			static void push(napi_env e) {
				NodeJS::env_ = e;
			}
			static void pop() {
				NodeJS::env_ = nullptr;
			}

			class Scope {
			public:
				Scope(napi_env e) {
					NodeJS::push(e);
				}
				~Scope() {
					NodeJS::pop();
				}
			};
		};
	}	
}

#define __NAPI_ENV__ /*Node Api env*/ emscripten::internal::NodeJS::env()

//#define EMSCRIPTEN_INTERNAL_COMMON_INIT \
//		napi_env emscripten::internal::NodeJS::env_ = nullptr;

#define NODE_EMBIND_EMSCRIPTEN_COMMON_INIT \
		napi_env emscripten::internal::NodeJS::env_ = nullptr;

#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_COMMON_H_