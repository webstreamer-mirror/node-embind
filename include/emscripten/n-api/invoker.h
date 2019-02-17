#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_H_


#include <emscripten/n-api/value.h>
namespace emscripten {
	namespace internal {
		namespace napi {

			template<typename ReturnType, typename... Args>
			struct MemberInvoker
			{};

			template<typename ReturnType>
			struct MemberInvoker <ReturnType>
			{
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	                            ReturnType(ClassType::*fn)() ) {
				//	return  (inst->*fn)();
				//}
				//
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	                            ReturnType(ClassType::*fn)() const ) {
				//	return  (inst->*fn)();
				//}

				// @
				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)()) {
					return  (inst.*fn)();
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)() const) {
					return  (inst.*fn)();
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)()) {
					return  (inst.*fn)();
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)() const) {
					return  (inst.*fn)();
				}

			};

			template<typename ReturnType, typename T0>
			struct MemberInvoker <ReturnType, T0>
			{
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	ReturnType(ClassType::*fn)(T0) ) {
				//
				//	return  (inst->*fn)(
				//		napi::value<T0>(env).native_cast(argv[0])
				//		);
				//}
				//
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	 ReturnType(ClassType::*fn)(T0) const) {
				//	return  (inst->*fn)(
				//		napi::value<T0>(env).native_cast(argv[0])
				//		);
				//}

				// @
				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)(T0)) {

					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)(T0) const) {
					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)(T0)) {

					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)(T0) const) {
					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}
			};

			template<typename ReturnType, typename T0, typename T1>
			struct MemberInvoker <ReturnType, T0, T1>
			{
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	ReturnType(ClassType::*fn)(T0,T1)) {
				//
				//	return  (inst->*fn)(
				//		napi::value<T0,T1>(env).native_cast(argv[0], argv[1])
				//		);
				//}
				//
				//template<typename ClassType>
				//inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	ReturnType(ClassType::*fn)(T0,T1) const) {
				//	return  (inst->*fn)(
				//		napi::value<T0,T1>(env).native_cast(argv[0], argv[1])
				//		);
				//}

				// @
				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)(T0, T1)) {

					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0]),
						napi::value<T1>(env).native_cast(argv[1])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType& inst,
					ReturnType(ClassType::*fn)(T0, T1) const) {
					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0]),
						napi::value<T1>(env).native_cast(argv[1])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)(T0, T1)) {

					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0]),
						napi::value<T1>(env).native_cast(argv[1])
						);
				}

				template<typename ClassType>
				inline static ReturnType invoke(napi_env env, napi_value argv[], const ClassType& inst,
					ReturnType(ClassType::*fn)(T0, T1) const) {
					return  (inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0]),
						napi::value<T1>(env).native_cast(argv[1])
						);
				}

			};

			template<typename T0>
			struct MemberInvoker <void, T0>
			{
				//template<typename ClassType>
				//inline static void invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	void (ClassType::*fn)(T0)) {
				//	
				//	
				//	(inst->*fn)(
				//		napi::value<T0>(env).native_cast(argv[0])
				//		);
				//}
				//
				//template<typename ClassType>
				//inline static void invoke(napi_env env, napi_value argv[], ClassType* inst,
				//	void (ClassType::*fn)(T0) const) {
				//	 (inst->*fn)(
				//		 napi::value<T0>(env).native_cast(argv[0])
				//		);
				//}

				//
				template<typename ClassType>
				inline static void invoke(napi_env env, napi_value argv[], ClassType& inst,
					void (ClassType::*fn)(T0)) {


					(inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static void invoke(napi_env env, napi_value argv[], ClassType& inst,
					void (ClassType::*fn)(T0) const) {
					(inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static void invoke(napi_env env, napi_value argv[], const ClassType& inst,
					void (ClassType::*fn)(T0)) {


					(inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}

				template<typename ClassType>
				inline static void invoke(napi_env env, napi_value argv[], const ClassType& inst,
					void (ClassType::*fn)(T0) const) {
					(inst.*fn)(
						napi::value<T0>(env).native_cast(argv[0])
						);
				}
			};



        //====================================
	    //    Invoker
		//====================================

				template<typename ReturnType, typename... Args>
				struct Invoker
				{};

				template<typename ReturnType>
				struct Invoker <ReturnType>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)()) {
						return fn();
					}

				};

				template<typename ReturnType, typename T0>
				struct Invoker <ReturnType, T0>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0)) {
						return fn(
							napi::value<T0>(env).native_cast(argv[0])
						);
					}
				};

				template<typename ReturnType, typename T0, typename T1>
				struct Invoker <ReturnType, T0, T1>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0, T1)) {

						return fn(
							napi::value<T0>(env).native_cast(argv[0]),
							napi::value<T1>(env).native_cast(argv[1])

						);
					}
				};

				template<typename ReturnType, typename T0, typename T1, typename T2>
				struct Invoker <ReturnType, T0, T1, T2>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], ReturnType(*fn)(T0, T1, T2)) {

						return fn(
							napi::value<T0>(env).native_cast(argv[0]),
							napi::value<T1>(env).native_cast(argv[1]),
							napi::value<T2>(env).native_cast(argv[2])

						);
					}
				};

    //=====================================
	// Function Invoker
	//=======================

				template<typename ReturnType, typename T, typename... Args>
				struct FunctionInvoker
				{};

				template<typename ReturnType, typename T>
				struct FunctionInvoker <ReturnType, T>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], T v, ReturnType(*fn)(T)) {
						return fn(v);
					}

				};

				template<typename ReturnType, typename T, typename T0>
				struct FunctionInvoker <ReturnType, T, T0>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], T v, ReturnType(*fn)(T, T0)) {
						return fn(v,
							::napi::value<T0>(env).native_cast(argv[0])
						);
					}
				};

				template<typename ReturnType, typename T, typename T0, typename T1>
				struct FunctionInvoker <ReturnType, T, T0, T1>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], T v, ReturnType(*fn)(T, T0, T1)) {

						return fn(v,
							::napi::value<T0>(env).native_cast(argv[0]),
							::napi::value<T1>(env).native_cast(argv[1])

						);
					}
				};

				template<typename ReturnType, typename T, typename T0, typename T1, typename T2>
				struct FunctionInvoker <ReturnType, T, T0, T1, T2>
				{
					inline static ReturnType invoke(napi_env env, napi_value argv[], T v, ReturnType(*fn)(T0, T1, T2)) {

						return fn(v,
							::napi::value<T0>(env).native_cast(argv[0]),
							::napi::value<T1>(env).native_cast(argv[1]),
							::napi::value<T2>(env).native_cast(argv[2])

						);
					}
				};

				// void
				template<typename T>
				struct FunctionInvoker <void, T>
				{
					inline static void invoke(napi_env env, napi_value argv[], T v, void(*fn)(T)) {
						return fn(v);
					}

				};

				template<typename T, typename T0>
				struct FunctionInvoker <void, T, T0>
				{
					inline static void invoke(napi_env env, napi_value argv[], T v, void(*fn)(T, T0)) {
						fn(v,
							::napi::value<T0>(env).native_cast(argv[0])
						);
					}
				};

				template<typename T, typename T0, typename T1>
				struct FunctionInvoker <void, T, T0, T1>
				{
					inline static void invoke(napi_env env, napi_value argv[], T v, void(*fn)(T, T0, T1)) {

						fn(v,
							::napi::value<T0>(env).native_cast(argv[0]),
							::napi::value<T1>(env).native_cast(argv[1])
						);
					}
				};

				template<typename T, typename T0, typename T1, typename T2>
				struct FunctionInvoker <void, T, T0, T1, T2>
				{
					inline static void invoke(napi_env env, napi_value argv[], T v, void(*fn)(T0, T1, T2)) {

						fn(v,
							::napi::value<T0>(env).native_cast(argv[0]),
							::napi::value<T1>(env).native_cast(argv[1]),
							::napi::value<T2>(env).native_cast(argv[2])

						);
					}
				};

		}
	}
}
#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_H_