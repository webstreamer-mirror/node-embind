#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_CLASS_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_CLASS_H_

#include <emscripten/n-api/common.h>


NS_NAPI_BEGIN

namespace convertor {

    template<typename T>
    inline T* GetObject(const context_t& ctx, napi_value val)
    {
        napi::Class<T>* wobj = nullptr;
        napi_status status = napi_unwrap(ctx.env, val, (void**)&wobj);

        NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

        return wobj->instance;
    }

    template<typename T>
    inline static napi_value CreateObject(const context_t& ctx, T* obj, bool by_val = true /*or by reference*/) {

        napi_value ctor = nullptr;
        class_t* prototype = napi::Class<T>::prototype;
        napi_status status = napi_get_reference_value(ctx.env, prototype->ref, &ctor);
        NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

        size_t argc = 2;
        napi_value argv[2] = { nullptr,nullptr };
        napi_value instance;
        napi_create_external(ctx.env, obj, nullptr, nullptr, &argv[0]);
        
        napi_get_boolean(ctx.env, by_val, &argv[1]);
        

        status = napi_new_instance(ctx.env, ctor, argc, argv, &instance);
        NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
        return instance;

    }

	template<typename T>
	struct Class;

    template<typename T>
    struct Class {
        typedef T type;
        type*     value_;

        type& value() { return *this->value_; }
        

        Class (const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx,val);
        }

        inline static napi_value napivalue(const context_t& ctx, type val) {

            return CreateObject<T>(ctx, &val, true);
        }
    };

    template<typename T>
    struct Class <T&> {
        typedef T type;
        type*     value_;

        type& value() { return *this->value_; }


        Class(const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx, val);
        }

        inline static napi_value napivalue(const context_t& ctx, T& val) {

            return CreateObject<T>(ctx, &val, false);
        }
    };

    template<typename T>
    struct Class <const T&> {
        typedef const T& type;
        T*     value_;

        const T& value() { return *this->value_; }


        Class(const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx, val);
        }

        inline static napi_value napivalue(const context_t& ctx, type val) {

            return CreateObject<T>(ctx, const_cast<T*>(&val), false);
        }
    };

    template<typename T>
    struct Class <T*> {
        T*     value_;

        T* value() { return this->value_; }


        Class(const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx, val);
        }

        inline static napi_value napivalue(const context_t& ctx, T* val) {

            return CreateObject<T>(ctx, val, false);

        }
    };

    template<typename T>
    struct Class <const T*> {
        typedef const T* type;
        T*     value_;

        const T* value() { return this->value_; }


        Class(const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx, val);
        }

        inline static napi_value napivalue(const context_t& ctx, type val) {

            return CreateObject<T>(ctx, const_cast<T*>(val), false);

        }
    };

    template<typename T>
    struct Class <T&&> {
        typedef T type;
        type*     value_;

        type& value() { return *this->value_; }


        Class(const context_t& ctx, napi_value val) {
            this->value_ = GetObject<T>(ctx, val);
        }

        inline static napi_value napivalue(const context_t& ctx, type val) {

            return CreateObject<T>(ctx, &val, false);
        }
    };
}
NS_NAPI_END

#endif // ! _NODE_EMBIND_EMSCRIPTEN_NAPI_CONVERTOR_FUNDAMENTAL_H_