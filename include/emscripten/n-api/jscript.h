#pragma once

#include <emscripten/n-api/common.h>

NS_NAPI_BEGIN

//
//  JArguments is used to manage the arguments excahnge between javascript and n-api
//  we put the c++ arguments to glolbal.__embind.${adress} object for 
//  $0 $1 $2 and result for function return value
struct JArguments {

    JArguments(const context_t& ctx)
        : context_(ctx), args_(nullptr)
    {
        sprintf(address_, "$%p", this);
        this->args();
    }

    template<typename T0>
    JArguments(const context_t& ctx, T0 v0)
        : context_(ctx), args_(nullptr)
    {
        sprintf(address_, "$%p", this);
        argv(0, v0);
    }

    template<typename T0, typename T1>
    JArguments(const context_t& ctx, T0 v0, T1 v1)
        : context_(ctx), args_(nullptr)
    {
        sprintf(address_, "$%p", this);
        argv(0, v0);
        argv(1, v1);
    }

    template<typename T0, typename T1, typename T2>
    JArguments(const context_t& ctx, T0 v0, T1 v1, T2 v2)
        : context_(ctx), args_(nullptr)
    {
        sprintf(address_, "$%p", this);
        argv(0, v0);
        argv(1, v1);
        argv(2, v2);
    }


    const char* name() const { return address_; }

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


    inline napi_value args(bool auto_create = true) {
        if (args_ == nullptr) {
            if (auto_create) {
                // create global object for the arguments
                // global.__node_embind.${address}
                napi_value global;
                napi_get_global(context_.env, &global);
                napi_value em = Property(global, "__node_embind");
                args_ = Property(em, address_); // __node_embind.${address}
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

    inline napi_value Property(napi_value obj, const char* name, bool auto_create = true) {
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
    char address_[128];
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
        JArguments jargs(context_, args...);
        char expr[256];
        sprintf(expr, "var $this = global.__node_embind.%s;" JSCRIPT_LINE_END, jargs.name());
        

        std::string jscript(expr);
        jscript += "$this.fn=function(){" JSCRIPT_LINE_END;

        for (int i = 0; i < (int)(sizeof...(Args)); i++) {
            if ((int)argv.size() > i) {
                sprintf(expr, "var %s = $this.$%d;" JSCRIPT_LINE_END, argv[i].c_str(), i);
            }
            else {
                sprintf(expr, "var $%d = $this.$%d;" JSCRIPT_LINE_END, i, i);
            }
            jscript += expr;
        }
        
        jscript += script_;

        sprintf(expr, "};" JSCRIPT_LINE_END	"$this.result =$this.fn();" JSCRIPT_LINE_END);

        jscript += expr;

        napi_value nscript, result;
        napi_status status = napi_create_string_latin1(context_.env, jscript.data(), jscript.size(), &nscript);
        status = napi_run_script(context_.env, nscript, &result);
        assert(status == napi_ok);
        return jargs.result();

    }

};

NS_NAPI_END