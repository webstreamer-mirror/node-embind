#pragma once

#include <emscripten/n-api/common.h>

NS_NAPI_BEGIN

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
        napi_set_property(env_, obj, key, napi::value<T>(env_).napi_cast(v));
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


struct JScript {
    JScript(napi_env env, const char* script)
        :env_(env), script_(script)
    {}

    JScript(const char* script)
        :env_(VM::get_napi_env()), script_(script)
    {}

    napi_env env_;
    const char* script_;

    template<typename ...Args>
    napi_value Call(Args... args) {
        return this->call(std::vector<std::string>(), args...);
    }

    template<typename ...Args>
    napi_value call(const std::vector<std::string>& argv, Args... args) {
        StackPointer sp(env_, args...);

        char expr[256];
        sprintf(expr, "var $sp = global.__emscripten.stack_pointer['%s'];", sp.value());

        std::string jscript(expr);
        jscript += "$sp.fn=function(){";

        for (int i = 0; i < (int)(sizeof...(Args)); i++) {
            if ((int)argv.size() > i) {
                sprintf(expr, "var %s = $sp.$%d;", argv[i].c_str(), i);
            }
            else {
                sprintf(expr, "var $%d = $sp.$%d;", i, i);
            }
            jscript += expr;
        }

        jscript += script_;

        sprintf(expr, "};"	"$sp.result =$sp.fn();");

        jscript += expr;

        napi_value nscript, result;
        napi_status status = napi_create_string_latin1(env_, jscript.data(), jscript.size(), &nscript);
        status = napi_run_script(env_, nscript, &result);
        assert(status == napi_ok);
        return sp.result();
    }

};

NS_NAPI_END