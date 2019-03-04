#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_CLASS_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_CLASS_H_

#include <emscripten/n-api/common.h>


NS_NAPI_BEGIN

namespace convertor {

    template<typename T>
    inline T* GetObject(napi_env env, napi_value val)
    {
        napi::Class<T>* wobj = nullptr;
        napi_status status = napi_unwrap(env, val, (void**)&wobj);

        NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);

        return wobj->instance;
    }

    template<typename T>
    inline static napi_value CreateObject(napi_env env, T* obj, bool by_val = true /*or by reference*/) {

        napi_value ctor = nullptr;
        class_t* prototype = napi::Class<T>::prototype;
        napi_status status = napi_get_reference_value(env, prototype->ref, &ctor);
        NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);

        size_t argc = 2;
        napi_value argv[2] = { nullptr,nullptr };
        napi_value instance;
        napi_create_external(env, obj, nullptr, nullptr, &argv[0]);
        
        napi_get_boolean(env, by_val, &argv[1]);
        

        status = napi_new_instance(env, ctor, argc, argv, &instance);
        NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status);
        return instance;

    }

	template<typename T>
	struct Class;

    template<typename T>
    struct Class {
        typedef T type;
        type*     value_;

        type& value() { return *this->value_; }
        

        Class (napi_env env, napi_value val) {
            this->value_ = GetObject<T>(env,val);
        }

        inline static napi_value napivalue(napi_env env, type val) {

            return CreateObject<T>(env, &val, true);
        }
    };

    template<typename T>
    struct Class <T&> {
        typedef T type;
        type*     value_;

        type& value() { return *this->value_; }


        Class(napi_env env, napi_value val) {
            this->value_ = GetObject<T>(env, val);
        }

        inline static napi_value napivalue(napi_env env, T& val) {

            return CreateObject<T>(env, &val, false);

        }
    };

    template<typename T>
    struct Class <T*> {
        T*     value_;

        T* value() { return this->value_; }


        Class(napi_env env, napi_value val) {
            this->value_ = GetObject<T>(env, val);
        }

        inline static napi_value napivalue(napi_env env, T* val) {

            return CreateObject<T>(env, &val, false);

        }
    };

}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_