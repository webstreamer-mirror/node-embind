 #pragma once

#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <node_api.h>


namespace emscripten {
	namespace napi {
		namespace internal {
			template<typename T>
			struct Class {
				typedef T type;
				napi_env env_;
				//type     value_;
				Class(napi_env env)
					: env_(env)
				{}

				inline T& native_cast(napi_value val)
				{
					typedef remove_decorate<T>::type ClassType;
					napi_valuetype type;
					napi_status status;
					napi_env env = __NAPI_ENV__;
					napi_typeof(env, val, &type);
					emscripten::internal::Class<ClassType>* obj;
					
					status = napi_typeof(env, val, &type);
					status = napi_unwrap(env, val, (void**)&obj);
					return *obj->instance;

				}

				inline napi_value napi_cast(type v) {

					typedef remove_decorate<T>::type ClassType;
					return emscripten::internal::Class<ClassType>::New_(v);
				}

			};

#if 0
			template<>
			struct Class <emscripten::val> {
				typedef emscripten::val type;
				napi_env env_;
				Class(napi_env env)
					: env_(env)
				{}
			
				//inline type native_cast(napi_value val)
				//{
				//	napi_get_value_bool(env_, val, &value_);
				//	return value_;
				//}
			
				inline napi_value napi_cast(type val) {
					emscripten::internal::_emval_decref(val.handle);
					return val.handle->value;
				}
			};
#endif
			template<>
			struct Class <std::string> {
				typedef std::string type;
				napi_env env_;
				Class(napi_env env)
					: env_(env)
				{}

				inline type native_cast(napi_value val)
				{
					std::string result;
					napi_valuetype vtype;
					napi_typeof(env_, val, &vtype);
					if (vtype == napi_string) {
						size_t len=0,outlen;
						napi_get_value_string_utf8(env_, val, nullptr, 0, &len);
						len += 1;
						result.resize(len);
						napi_get_value_string_utf8(env_, val, (char*)result.data(), len, &outlen);
						result.resize(outlen);
					}
					return result;
				}

				inline napi_value napi_cast(type val) {
					napi_value value;
					napi_create_string_utf8(env_, val.data(), val.size(), &value);					
					return value;
				}
			};

			template<typename T>
			struct Class<std::unique_ptr<T>> {
				typedef std::unique_ptr<T> type;
				napi_env env_;
				Class(napi_env env)
					: env_(env)
				{}
				inline type native_cast(napi_value val)
				{
					T* result;
					napi_get_value_external(env_, val, (void**)&result);
					//napi_create_external(env_, val.release(), nullptr, nullptr, &result);
					return type(result);
				}

				inline napi_value napi_cast(type val) {
					napi_value result;
					napi_create_external(env_, val.release(), nullptr, nullptr, &result);
					return result;
				}

				//typedef typename BindingType<T*>::WireType WireType;
				//
				//static WireType toWireType(std::unique_ptr<T> p) {
				//	return BindingType<T*>::toWireType(p.release());
				//}
				//
				//static std::unique_ptr<T> fromWireType(WireType wt) {
				//	return std::unique_ptr<T>(BindingType<T*>::fromWireType(wt));
				//}
			};


			//
			template<typename T>
			struct Class<const T> : public Class<T> {
				Class(napi_env env)
					: Class<T>(env)
				{}
			};

			template<typename T>
			struct Class<T&> : public Class<T> {
				Class(napi_env env)
					: Class<T>(env)
				{}
			};

			template<typename T>
			struct Class<const T&> : public Class<T> {
				Class(napi_env env)
					: Class<T>(env)
				{}
			};

			template<typename T>
			struct Class<T&&> {
				typedef typename Class<T>::type type;
				Class<T> obj_;
				Class(napi_env env)
					: obj_(env)
				{}

				inline type native_cast(napi_value val)
				{
					return obj_.native_cast(val);
				}

				inline napi_value napi_cast(type val) {
					return obj_.napi_cast(val);
				}
			};


		}
	}
}