#pragma once

#include <emscripten/n-api/common.h>

NS_NAPI_BEGIN
#if 0
struct StackPointer {

    StackPointer(napi_env env)
        : env_(env), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        this->args();
    }

    template<typename T0>
    StackPointer(napi_env env, T0 v0)
        : env_(env), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
    }

    template<typename T0, typename T1>
    StackPointer(napi_env env, T0 v0, T1 v1)
        : env_(env), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
        argv(1, v1);
    }

    template<typename T0, typename T1, typename T2>
    StackPointer(napi_env env, T0 v0, T1 v1, T2 v2)
        : env_(env), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
        argv(1, v1);
        argv(2, v2);
    }

    const char* value() const { return value_; }
    inline napi_value sp() {
        if (sp_ == nullptr) {
            napi_value global;
            napi_get_global(env_, &global);
            napi_value em = get_property(global, "__emscripten", true);
            sp_ = get_property(em, "stack_pointer", true);
        }
        return sp_;
    }

    inline napi_value args(bool auto_create = true) {
        if (args_ == nullptr) {
            if (auto_create) {
                napi_value key;
                napi_create_string_latin1(env_, value_, NAPI_AUTO_LENGTH, &key);
                napi_create_object(env_, &args_);
                napi_set_property(env_, sp(), key, args_);
            }
        }
        return args_;
    }

    template<typename T>
    void argv(int i, T v) {
        napi_value obj = this->args();
        napi_value key;
        char name[32];
        sprintf(name, "$%d", i);
        napi_create_string_latin1(env_, name, NAPI_AUTO_LENGTH, &key);
        napi_set_property(env_, obj, key, napi::value<T>(env_,v).value());
    }

    napi_value result() {
        napi_value obj = args(false);
        bool has;
        if (obj) {
            napi_value key;
            napi_create_string_latin1(env_, "result", NAPI_AUTO_LENGTH, &key);
            napi_has_property(env_, obj, key, &has);
            if (has) {
                napi_value v;
                napi_get_property(env_, obj, key, &v);
                return v;
            }
        }
        return nullptr;
    }

protected:
    inline napi_value get_property(napi_value obj, const char* name, bool auto_create = true) {
        napi_value key, val = nullptr;
        napi_create_string_latin1(env_, name, NAPI_AUTO_LENGTH, &key);
        bool has;
        napi_has_property(env_, obj, key, &has);
        if (has) {
            napi_get_property(env_, obj, key, &val);
        }
        else {
            if (auto_create) {
                napi_create_object(env_, &val);
                napi_set_property(env_, obj, key, val);
            }
        }
        return val;
    }

    napi_env env_;
    char value_[64];
    napi_value sp_;
    napi_value args_;

};
#endif
struct JSContext {

    JSContext(const context_t& ctx)
        : context_(ctx), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        this->args();
    }

    template<typename T0>
    JSContext(const context_t& ctx, T0 v0)
        : context_(ctx), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
    }

    template<typename T0, typename T1>
    JSContext(const context_t& ctx, T0 v0, T1 v1)
        : context_(ctx), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
        argv(1, v1);
    }

    template<typename T0, typename T1, typename T2>
    JSContext(const context_t& ctx, T0 v0, T1 v1, T2 v2)
        : context_(ctx), sp_(nullptr), args_(nullptr)
    {
        sprintf(value_, "%p", this);
        argv(0, v0);
        argv(1, v1);
        argv(2, v2);
    }


    const char* value() const { return value_; }
    inline napi_value sp() {
        if (sp_ == nullptr) {
            napi_value global;
            napi_get_global(context_.env, &global);
            napi_value em = get_property(global, "__node_embind", true);
            sp_ = get_property(em, "$sp", true);
        }
        return sp_;
    }

    inline napi_value args(bool auto_create = true) {
        if (args_ == nullptr) {
            if (auto_create) {
                napi_value key;
                napi_create_string_latin1(context_.env, value_, NAPI_AUTO_LENGTH, &key);
                napi_create_object(context_.env, &args_);
                napi_set_property(context_.env, sp(), key, args_);
            }
        }
        return args_;
    }

    template<typename T>
    void argv(int i, T v) {
        napi_value obj = this->args();
        napi_value key;
        char name[32];
        sprintf(name, "$%d", i);
        napi_create_string_latin1(context_.env, name, NAPI_AUTO_LENGTH, &key);
        napi_set_property(context_.env, obj, key, 
            napi::value<T>::napivalue(context_, v));
    }

    napi_value result() {
        napi_value obj = args(false);
        bool has;
        if (obj) {
            napi_value key;
            napi_create_string_latin1(context_.env, "result", NAPI_AUTO_LENGTH, &key);
            napi_has_property(context_.env, obj, key, &has);
            if (has) {
                napi_value v;
                napi_get_property(context_.env, obj, key, &v);
                return v;
            }
        }
        return nullptr;
    }

protected:
    inline napi_value get_property(napi_value obj, const char* name, bool auto_create = true) {
        napi_value key, val = nullptr;
        napi_create_string_latin1(context_.env, name, NAPI_AUTO_LENGTH, &key);
        bool has;
        napi_has_property(context_.env, obj, key, &has);
        if (has) {
            napi_get_property(context_.env, obj, key, &val);
        }
        else {
            if (auto_create) {
                napi_create_object(context_.env, &val);
                napi_set_property(context_.env, obj, key, val);
            }
        }
        return val;
    }

    const context_t& context_;
    char value_[64];
    napi_value sp_;
    napi_value args_;
};

struct JScript {
    JScript(const context_t& ctx, const char* script)
        :context_(ctx), script_(script)
    {}

    const char* script_;
    const context_t& context_;

    template<typename ...Args>
    napi_value Call(Args... args) {
        return this->call(std::vector<std::string>(), args...);
    }

#define JSCRIPT_LINE_END "\n"
    template<typename ...Args>
    napi_value call(const std::vector<std::string>& argv, Args... args) {
        JSContext sp(context_, args...);
        char expr[256];
        sprintf(expr, "var $sp = global.__node_embind.$sp['%s'];" JSCRIPT_LINE_END, sp.value());
        

        std::string jscript(expr);
        jscript += "$sp.fn=function(){" JSCRIPT_LINE_END;

        for (int i = 0; i < (int)(sizeof...(Args)); i++) {
            if ((int)argv.size() > i) {
                sprintf(expr, "var %s = $sp.$%d;" JSCRIPT_LINE_END, argv[i].c_str(), i);
            }
            else {
                sprintf(expr, "var $%d = $sp.$%d;" JSCRIPT_LINE_END, i, i);
            }
            jscript += expr;
        }
        
        jscript += script_;

        sprintf(expr, "};" JSCRIPT_LINE_END	"$sp.result =$sp.fn();" JSCRIPT_LINE_END);

        jscript += expr;

        napi_value nscript, result;
        napi_status status = napi_create_string_latin1(context_.env, jscript.data(), jscript.size(), &nscript);
        status = napi_run_script(context_.env, nscript, &result);
        assert(status == napi_ok);
        return sp.result();

    }

};

NS_NAPI_END