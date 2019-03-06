#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_FUNCTION_INVOKER_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_FUNCTION_INVOKER_H_

#include <emscripten/n-api/common.h>
#include <emscripten/n-api/value.h>

#ifndef _arg
#define _arg(n) napi::value<T##n>(ctx.env, ctx.argv[n]).value()
#endif


NS_NAPI_BEGIN


template<typename ReturnType, typename... Args>
struct FunctionInvoker;

template<typename ReturnType>
struct FunctionInvoker <ReturnType>
{
    template<typename ClassType>
    inline static ReturnType invoke(const context_t& ctx, ClassType* inst,
        ReturnType(*fn)(ClassType&)) {
        return  fn(*inst);
    }

};

template<typename ReturnType, typename T0>
struct FunctionInvoker <ReturnType, T0>
{
    template<typename ClassType>
    inline static ReturnType invoke(const context_t& ctx, ClassType* inst,
        ReturnType(*fn)(ClassType&, T0)) {
        return  fn(*inst, _arg(0));
    }

};

template<typename ReturnType, typename T0, typename T1>
struct FunctionInvoker <ReturnType, T0, T1>
{
    template<typename ClassType>
    inline static ReturnType invoke(const context_t& ctx, ClassType* inst,
        ReturnType(*fn)(ClassType&, T0, T1)) {
        return  fn(*inst, _arg(0), _arg(1));
    }

};

NS_NAPI_END

#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_INVOKER_FUNCTION_INVOKER_H_