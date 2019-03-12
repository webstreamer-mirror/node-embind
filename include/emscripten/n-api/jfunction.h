#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_JFUNCTION_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_JFUNCTION_H_

#include <emscripten/n-api/common.h>

NS_NAPI_BEGIN


struct _JFunctionBase {
    static std::vector<std::string> tokenize(const std::string& src, const std::string& tok)
    {
        using namespace std;
        vector<string> v;

        if (src.empty() || tok.empty()) return v;

        size_t n = 0, pos = 0, len = 0;

        while ((pos = src.find_first_of(tok, n)) != string::npos)
        {
            if ((len = pos - n) != 0)
                v.push_back(src.substr(n, len));
            n = pos + 1;
        }
        string last = src.substr(n);
        if (!last.empty())	v.push_back(last);
        return v;
    }

    static bool is_key_word(const std::string& name) {
        return name == "void" || name == "int";
    }
    template<typename... Args>
    static napi_value call_(context_t& ctx, const char* params, const char* script, Args... args) {
        using namespace emscripten;
        using namespace emscripten::internal;
        std::vector<std::string> argv;

        if (params) {
            std::vector<std::string> fields = tokenize(params, "(),");
            for (std::vector<std::string>::iterator it = fields.begin();
                it != fields.end(); it++) {
                std::vector<std::string> words = tokenize(*it, " ");
                std::string& name = words.back();
                if (!name.empty() && !is_key_word(name))
                {
                    argv.push_back(words.back());
                }
            }

        }
        return JScript(ctx,script).call(argv, args...);
    }
};

template<typename ReturnType>
struct _JFunction : public _JFunctionBase {

    template<typename... Args>
    static ReturnType Call(const char* params, const char* script, Args... args) {
        context_t* ctx = VM::context();
        assert(ctx && ctx->env);

        return napi::value<ReturnType>(*ctx, call_(*ctx, params, script, args...)).value();
    }
};

template<>
struct _JFunction<void> : public _JFunctionBase {

    template<typename... Args>
    static void Call(const char* params, const char* script, Args... args) {
        context_t* ctx = VM::context();
        assert(ctx && ctx->env);
        call_(*ctx,params, script, args...);
    }

};

NS_NAPI_END

#endif // !_NODE_EMBIND_EMSCRIPTEN_NAPI_JFUNCTION_H_