#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_MEMBER_INVOKER_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_MEMBER_INVOKER_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/value.h>

#ifndef _arg
#define _arg(n) napi::value<T##n>(env, argv[n]).value()
#endif


NS_NAPI_BEGIN


template<typename ReturnType, typename... Args>
struct MemberInvoker;

template<typename ReturnType>
struct MemberInvoker <ReturnType>
{
    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)()) {
        return  (inst->*fn)();
    }

    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)() const) {
        return  (inst->*fn)();
    }
};

template<typename ReturnType, typename T0>
struct MemberInvoker <ReturnType, T0>
{
    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)(T0)) {

        return  (inst->*fn)( _arg(0) );
    }

    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)(T0) const) {
        return  (inst->*fn)(_arg(0));
    }
};

template<typename ReturnType, typename T0, typename T1>
struct MemberInvoker <ReturnType, T0, T1>
{
    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)(T0,T1)) {

        return  (inst->*fn)(_arg(0),_arg(1));
    }

    template<typename ClassType>
    inline static ReturnType invoke(napi_env env, napi_value argv[], ClassType* inst,
        ReturnType(ClassType::*fn)(T0,T1) const) {
        return  (inst->*fn)(_arg(0),_arg(1));
    }
};


NS_NAPI_END

#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_MEMBER_INVOKER_H_