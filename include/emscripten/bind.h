
#ifndef _NODE_EMBIND_EMSCRIPTEN_BIND_H_
#define _NODE_EMBIND_EMSCRIPTEN_BIND_H_

// EMBIND_STD_STRING_IS_UTF8

#include <stddef.h>
#include <assert.h>
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <type_traits>
#include <stdio.h>

#include <tuple>

#define EMSCRIPTEN_NODE_EMBIND

#include <emscripten/wire.h>

namespace emscripten {

    namespace internal {
        typedef long GenericEnumValue;
        typedef void* GenericFunction;
        typedef void(*VoidFunctionPtr)(void);
    }
}

#include <emscripten/n-api/n-napi.h>

namespace emscripten {
    ////////////////////////////////////////////////////////////////////////////////
    // POLICIES
    ////////////////////////////////////////////////////////////////////////////////

    template<int Index>
    struct arg {
        static constexpr int index = Index + 1;
    };

    struct ret_val {
        static constexpr int index = 0;
    };



    // whitelist all raw pointers
    struct allow_raw_pointers {
        template<typename InputType, int Index>
        struct Transform {
            typedef typename std::conditional<
                std::is_pointer<InputType>::value,
                internal::AllowedRawPointer<typename std::remove_pointer<InputType>::type>,
                InputType
            >::type type;
        };
    };

    // this is temporary until arg policies are reworked
    template<typename Slot>
    struct allow_raw_pointer : public allow_raw_pointers {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // select_overload and select_const
    ////////////////////////////////////////////////////////////////////////////////

    template<typename Signature>
    Signature* select_overload(Signature* fn) {
        return fn;
    }

    template<typename Signature, typename ClassType>
    auto select_overload(Signature(ClassType::*fn)) -> decltype(fn) {
        return fn;
    }

    template<typename ClassType, typename ReturnType, typename... Args>
    auto select_const(ReturnType(ClassType::*method)(Args...) const) -> decltype(method) {
        return method;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Invoker
    ////////////////////////////////////////////////////////////////////////////////

    namespace internal {
        template<typename ReturnType, typename... Args>
        struct Invoker {
            typedef ReturnType(*Fn)(Args...);

            typedef napi::Invoker<ReturnType, Args...> I;

            static napi_value invoke(
                const napi::function_t* self,
                const napi::context_t& ctx)
            {
                if (self->argc == ctx.argc) {
                    return napi::value<ReturnType>::napivalue(ctx, I::invoke(ctx, (Fn)self->function));
                }

                // recursion to find out override functin
                napi::function_t* fn = self->next;
                while (fn) {
                    if (fn->argc == ctx.argc) {
                        return fn->invoke(fn, ctx);
                    }
                }

                // no match overwrite function match the provide arguments.
                napi_throw_error(ctx.env, NODE_EMBIND_ERROR_ARGC, NODE_EMBIND_ERROR_ARGC_MSG);
                return nullptr;
            }

        };

        template<typename... Args>
        struct Invoker<void, Args...> {

            typedef void(*Fn)(Args...);
            typedef napi::Invoker<void, Args...> I;

            static napi_value invoke(const napi::function_t* self, const napi::context_t& ctx)
            {
                if (self->argc == ctx.argc) {
                    I::invoke(ctx, (Fn)self->function);
                    return nullptr;
                }

                napi::function_t* fn = self->next;
                while (fn) {
                    if (fn->argc == ctx.argc) {
                        return fn->invoke(fn, ctx);
                    }
                }

                napi_throw_error(ctx.env, NODE_EMBIND_ERROR_ARGC, NODE_EMBIND_ERROR_ARGC_MSG);

                return nullptr;
            }

        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////////

    template<typename ReturnType, typename... Args, typename... Policies>
    void function(const char* name, ReturnType(*fn)(Args...), Policies...) {

        using namespace internal;

        auto invoker = &Invoker<ReturnType, Args...>::invoke;

        napi::register_function(
            name,
            sizeof...(Args),
            reinterpret_cast<GenericFunction>(invoker),
            reinterpret_cast<GenericFunction>(fn));

    }



    namespace internal {
        template<typename ClassType, typename... Args>
        ClassType* operator_new(Args&&... args) {
            return new ClassType(std::forward<Args>(args)...);
        }


        template<typename FunctionPointerType, typename ReturnType, typename ThisType, typename... Args>
        struct FunctionInvoker {

            typedef napi::FunctionInvoker<ReturnType, Args...> I;
            static napi_value invoke(const napi::function_t* prototype, const napi::context_t& ctx) {
                FunctionPointerType fn = reinterpret_cast<FunctionPointerType>(prototype->function);

                using ClassType = typename napi::nomakeup<ThisType>::type;
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                
                return napi::value<ReturnType>::napivalue(ctx, I::invoke(ctx, self->instance, fn));
            }
        };

        template<typename FunctionPointerType, typename ThisType, typename... Args>
        struct FunctionInvoker<FunctionPointerType, void, ThisType, Args...> {
        
            typedef napi::FunctionInvoker<void, Args...> I;
            static napi_value invoke(const napi::function_t* prototype, const napi::context_t& ctx)
            {
                FunctionPointerType fn = reinterpret_cast<FunctionPointerType>(prototype->function);
                using ClassType = typename napi::nomakeup<ThisType>::type;
                napi::Class<ClassType>* self = nullptr;
        
                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);
        
                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);
                I::invoke(ctx, self->instance, fn);
                return nullptr;
            }
        };



        template<typename MemberPointer,
            typename ReturnType,
            typename ThisType,
            typename... Args>
            struct MethodInvoker {

            typedef napi::MemberInvoker<ReturnType, Args...> I;

            static napi_value invoke(const napi::function_t* prototype, const napi::context_t& ctx)
            {
                const MemberPointer& method = *static_cast<MemberPointer*>(prototype->function);

                using ClassType = typename napi::nomakeup<ThisType>::type;
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                return napi::value<ReturnType>::napivalue(ctx, I::invoke(ctx, self->instance, method));
            }
        };

        template<typename MemberPointer,
            typename ThisType,
            typename... Args>
            struct MethodInvoker<MemberPointer, void, ThisType, Args...> {

            typedef napi::MemberInvoker<void, Args...> I;

            static napi_value invoke(const napi::function_t* prototype, const napi::context_t& ctx)
            {
                const MemberPointer& method = *static_cast<MemberPointer*>(prototype->function);
                
                using ClassType = typename napi::nomakeup<ThisType>::type;
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);
                I::invoke(ctx, self->instance, method);
                return nullptr;
            }
        };

        template<typename InstanceType, typename MemberType>
        struct MemberAccess {
            typedef MemberType InstanceType::*MemberPointer;

            template<typename ClassType>
            static napi_value get(const napi::property_t* prototype, const napi::context_t& ctx) {

                MemberPointer& field = *static_cast<MemberPointer*>(prototype->getter_context);
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                ClassType& inst = *self->instance;
                return napi::value<MemberType>::napivalue(ctx, inst.*field);
            }

            template<typename ClassType>
            static napi_value set(const napi::property_t* prototype, const napi::context_t& ctx) {

                MemberPointer& field = *static_cast<MemberPointer*>(prototype->setter_context);

                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                ClassType& inst = *self->instance;

                inst.*field = napi::value<MemberType>(ctx, ctx.argv[0]).value();
                return nullptr;

            }

            template<typename ClassType>
            static void object_set(const napi::property_t* prototype, const napi::context_t& ctx,
                void* pinst, napi_value value) {

                ClassType& inst = *static_cast<ClassType*>(pinst);
                MemberPointer& field = *static_cast<MemberPointer*>(prototype->setter_context);

                inst.*field = napi::value<MemberType>(ctx, value).value();
            }
        };

        template<typename FieldType>
        struct GlobalAccess {

            static napi_value get(const napi::property_t* self, const napi::context_t& ctx) {

                FieldType* field = (FieldType*)self->getter_context;

                return napi::value<FieldType>::napivalue(ctx,*field);

            }
            static napi_value set(const napi::property_t* self, const napi::context_t& ctx) {

                FieldType* field = (FieldType*)self->getter_context;

                *field = napi::value<FieldType>(ctx,ctx.argv[0]).value();
                return nullptr;

            }
        };

        // TODO: This could do a reinterpret-cast if sizeof(T) === sizeof(void*)
        template<typename T>
        inline T* getContext(const T& t) {
            // not a leak because this is called once per binding
            return new T(t);
        }



        template<typename T>
        struct GetterPolicy;

        template<typename GetterReturnType, typename GetterThisType>
        struct GetterPolicy<GetterReturnType(GetterThisType::*)() const> {
            typedef GetterReturnType ReturnType;
            typedef GetterReturnType(GetterThisType::*Context)() const;

            template<typename ClassType>
            static napi_value get(const napi::property_t* prototype, const napi::context_t& ctx) {
                Context context = *static_cast<Context*>(prototype->getter_context);
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                return napi::value<ReturnType>::napivalue(ctx,
                    napi::MemberInvoker<ReturnType>::invoke(ctx, self->instance, context));
            }

            static void* getContext(Context context) {
                return internal::getContext(context);
            }

        };

        template<typename GetterReturnType, typename GetterThisType>
        struct GetterPolicy<GetterReturnType(*)(const GetterThisType&)> {
            typedef GetterReturnType ReturnType;
            typedef GetterReturnType(*Context)(const GetterThisType&);

            template<typename ClassType>
            static napi_value get(const napi::property_t* prototype, const napi::context_t& ctx) {
                Context context = *static_cast<Context*>(prototype->getter_context);
                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                return napi::value<ReturnType>::napivalue(ctx, context(*self->instance));
            }

            static void* getContext(Context context) {
                return internal::getContext(context);
            }
        };

        template<typename T>
        struct SetterPolicy;

        template<typename SetterThisType, typename SetterArgumentType>
        struct SetterPolicy<void (SetterThisType::*)(SetterArgumentType)> {
            typedef SetterArgumentType ArgumentType;
            typedef void (SetterThisType::*Context)(SetterArgumentType);

            template<typename ClassType>
            static napi_value set(const napi::property_t* prototype, const napi::context_t& ctx) {
                Context context = *static_cast<Context*>(prototype->setter_context);

                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                typedef napi::MemberInvoker<void, SetterArgumentType> I;
                I::invoke(ctx, self->instance, context);
                return nullptr;
            }

            template<typename ClassType>
            static void object_set(const napi::property_t* prototype, const napi::context_t& ctx,
                void* pinst, napi_value value) {
                Context context = *static_cast<Context*>(prototype->setter_context);
                

                typedef napi::MemberInvoker<void, SetterArgumentType> I;
                napi::context_t c = ctx;
 
                c.argc = 1;
                c.argv = &value;
                I::invoke(c, static_cast<ClassType*>(pinst), context);
            }

            static void* getContext(Context context) {
                return internal::getContext(context);
            }
        };


        template<typename SetterThisType, typename SetterArgumentType>
        struct SetterPolicy<void(*)(SetterThisType&, SetterArgumentType)> {
            typedef SetterArgumentType ArgumentType;
            typedef void(*Context)(SetterThisType&, SetterArgumentType);

            template<typename ClassType>
            static napi_value set(const napi::property_t* prototype, const napi::context_t& ctx) {
                Context context = *static_cast<Context*>(prototype->setter_context);

                napi::Class<ClassType>* self = nullptr;

                napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
                NODE_EMBIND_ERROR_NAPICALL_CHECK(ctx.env, status);

                NODE_EMBIND_ERROR_INVALID_INSTANCE_CHECK(ctx.env, self->instance);

                context(*self->instance,
                    napi::value<SetterArgumentType>(ctx, ctx.argv[0]).value());
                return nullptr;
            }

            template<typename ClassType>
            static void object_set(const napi::property_t* prototype, const napi::context_t& ctx,
                void* pinst, napi_value value) {
                Context context = *static_cast<Context*>(prototype->setter_context);
                ClassType& inst = *static_cast<ClassType*>(pinst);

                context(inst, napi::value<SetterArgumentType>(ctx, value).value());
            }

            static void* getContext(Context context) {
                return internal::getContext(context);
            }
        };

        class noncopyable {
        protected:
            noncopyable() {}
            ~noncopyable() {}
        private:
            noncopyable(const noncopyable&) = delete;
            const noncopyable& operator=(const noncopyable&) = delete;
        };

        template<typename ClassType, typename ElementType>
        static napi_value get_by_index(const napi::property_t* prototype, const napi::context_t& ctx) {
            int index = (int)(int64_t)prototype->getter_context;
            napi::Class<ClassType>* self = nullptr;

            napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
            NODE_EMBIND_ERROR_NAPICALL_RETURN(ctx.env, status, nullptr);
            NODE_EMBIND_ERROR_INVALID_INSTANCE_RETURN(ctx.env, self->instance, nullptr);

            return napi::value<ElementType>::napivalue(ctx, (*self->instance)[index]);
        }

        template<typename ClassType, typename ElementType>
        static napi_value set_by_index(const napi::property_t* prototype, const napi::context_t& ctx) {
            int index = (int)(int64_t)prototype->setter_context;
            napi::Class<ClassType>* self = nullptr;

            napi_status status = napi_unwrap(ctx.env, ctx.js, reinterpret_cast<void**>(&self));
            NODE_EMBIND_ERROR_NAPICALL_RETURN(ctx.env, status,nullptr);
            NODE_EMBIND_ERROR_INVALID_INSTANCE_RETURN(ctx.env, self->instance, nullptr);

            (*self->instance)[index] = napi::value<ElementType>(ctx, ctx.argv[0]).value();

            return nullptr;
        }

        template<typename ClassType, typename ElementType>
        static napi_value object_set_by_index(const napi::property_t* prototype, const napi::context_t& ctx,
            void* pinst, napi_value value) {
            int index = (int)(int64_t)prototype->setter_context;
            ClassType& inst = *static_cast<ClassType*>(pinst);

            inst[index] = napi::value<ElementType>(ctx, value).value();

            return nullptr;
        }

    }

    template<int Index>
    struct index {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // VALUE TUPLES
    ////////////////////////////////////////////////////////////////////////////////

    template<typename ClassType>
    class value_array : public internal::noncopyable {
    public:
        typedef ClassType class_type;

        value_array(const char* name) {
            using namespace internal;

            auto New = napi::Class<ClassType>::New;
            auto Delete = napi::Class<ClassType>::Delete;
            napi::register_class(
                name,
                TypeID<ClassType>::get(),
                reinterpret_cast<GenericFunction>(New),
                reinterpret_cast<GenericFunction>(Delete),
                &napi::Class<ClassType>::prototype,
                napi::class_t::value_array);
        }

        template<typename InstanceType, typename ElementType>
        value_array& element(ElementType InstanceType::*field) {
            using namespace internal;

            auto getter = &MemberAccess<ClassType, ElementType>::template get<ClassType>;
            auto setter = &MemberAccess<ClassType, ElementType>::template set<ClassType>;
            auto object_setter = &MemberAccess<ClassType, ElementType>::template object_set<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                nullptr,
                reinterpret_cast<GenericFunction>(getter),
                getContext(field),
                reinterpret_cast<GenericFunction>(setter),
                getContext(field),
                reinterpret_cast<GenericFunction>(object_setter));

            return *this;
        }

        template<typename Getter, typename Setter>
        value_array& element(Getter getter, Setter setter) {
            using namespace internal;
            typedef GetterPolicy<Getter> GP;
            typedef SetterPolicy<Setter> SP;

            auto g = &GP::template get<ClassType>;
            auto s = &SP::template set<ClassType>;
            auto os = &SP::template object_set<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                nullptr,
                reinterpret_cast<GenericFunction>(g),
                getContext(getter),
                reinterpret_cast<GenericFunction>(s),
                getContext(setter),
                reinterpret_cast<GenericFunction>(os));

            return *this;
        }

        template<int Index>
        value_array& element(index<Index>) {
            using namespace internal;
            ClassType* null = 0;
            typedef typename std::remove_reference<decltype((*null)[Index])>::type ElementType;

            auto getter = &internal::get_by_index<ClassType, ElementType>;
            auto setter = &internal::set_by_index<ClassType, ElementType>;
            auto osetter = &internal::object_set_by_index<ClassType, ElementType>;

            napi::register_class_property(
                TypeID<ClassType>::get(),
                nullptr,
                reinterpret_cast<GenericFunction>(getter),
                reinterpret_cast<void*>(Index),
                reinterpret_cast<GenericFunction>(setter),
                reinterpret_cast<void*>(Index),
                reinterpret_cast<GenericFunction>(osetter));
            return *this;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // VALUE STRUCTS
    ////////////////////////////////////////////////////////////////////////////////

    template<typename ClassType>
    class value_object : public internal::noncopyable {
    public:
        typedef ClassType class_type;

        value_object(const char* name) {
            using namespace internal;

            auto New = napi::Class<ClassType>::New;
            auto Delete = napi::Class<ClassType>::Delete;
            napi::register_class(
                name,
                TypeID<ClassType>::get(),
                reinterpret_cast<GenericFunction>(New),
                reinterpret_cast<GenericFunction>(Delete),
                &napi::Class<ClassType>::prototype,
                napi::class_t::value_object);
        }

        template<typename InstanceType, typename FieldType>
        value_object& field(const char* fieldName, FieldType InstanceType::*field) {
            using namespace internal;

            auto getter = &MemberAccess<ClassType, FieldType>::template get<ClassType>;
            auto setter = &MemberAccess<ClassType, FieldType>::template set<ClassType>;
            auto object_setter = &MemberAccess<ClassType, FieldType>::template object_set<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(getter),
                getContext(field),
                reinterpret_cast<GenericFunction>(setter),
                getContext(field),
                reinterpret_cast<GenericFunction>(object_setter));

            return *this;
        }

        template<typename Getter, typename Setter>
        value_object& field(
            const char* fieldName,
            Getter getter,
            Setter setter
        ) {
            using namespace internal;
            typedef GetterPolicy<Getter> GP;
            typedef SetterPolicy<Setter> SP;

            auto g = &GP::template get<ClassType>;
            auto s = &SP::template set<ClassType>;
            auto os = &SP::template object_set<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(g),
                getContext(getter),
                reinterpret_cast<GenericFunction>(s),
                getContext(setter),
                reinterpret_cast<GenericFunction>(os));

            return *this;
        }

        template<int Index>
        value_object& field(const char* fieldName, index<Index>) {
            using namespace internal;
            ClassType* null = 0;
            typedef typename std::remove_reference<decltype((*null)[Index])>::type ElementType;

            auto getter = &internal::get_by_index<ClassType, ElementType>;
            auto setter = &internal::set_by_index<ClassType, ElementType>;
            auto osetter = &internal::object_set_by_index<ClassType, ElementType>;
            
            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(getter),
                reinterpret_cast<void*>(Index),
                reinterpret_cast<GenericFunction>(setter),
                reinterpret_cast<void*>(Index),
                reinterpret_cast<GenericFunction>(osetter) );
            return *this;
        }

    };

    ////////////////////////////////////////////////////////////////////////////////
    // CLASSES
    ////////////////////////////////////////////////////////////////////////////////

    namespace internal {
        struct NoBaseClass {
            template<typename ClassType>
            static void verify() {
            }

            static TYPEID get() {
                return nullptr;
            }

            template<typename ClassType>
            static VoidFunctionPtr getUpcaster() {
                return nullptr;
            }

            template<typename ClassType>
            static VoidFunctionPtr getDowncaster() {
                return nullptr;
            }
        };

        // NOTE: this returns the class type, not the pointer type
        template<typename T>
        inline TYPEID getActualType(T* ptr) {
            return getLightTypeID(*ptr);
        };
    }

    struct pure_virtual {
        template<typename InputType, int Index>
        struct Transform {
            typedef InputType type;
        };
    };

    namespace internal {
        template<typename... Policies>
        struct isPureVirtual;

        template<typename... Rest>
        struct isPureVirtual<pure_virtual, Rest...> {
            static constexpr bool value = true;
        };

        template<typename T, typename... Rest>
        struct isPureVirtual<T, Rest...> {
            static constexpr bool value = isPureVirtual<Rest...>::value;
        };

        template<>
        struct isPureVirtual<> {
            static constexpr bool value = false;
        };
    }

    namespace internal {
        template<typename ClassType, typename... Args>
        struct ConstructorInvoker {

            typedef ClassType* (*Fn)(Args...);
            typedef napi::Invoker<ClassType*, Args...> I;
            static void* invoke(const napi::constructor_t* self, const napi::context_t& ctx)
            {
                assert(ctx.argc == sizeof...(Args));
                return I::invoke(ctx, (Fn)self->function);
            }
        };
    }


    template<typename ClassType, typename BaseSpecifier = internal::NoBaseClass>
    class class_ {
    public:
        typedef ClassType class_type;
        typedef BaseSpecifier base_specifier;

        class_() = delete;

        explicit class_(const char* name) {
            using namespace internal;

            BaseSpecifier::template verify<ClassType>();

            auto New = napi::Class<ClassType>::New;
            auto Delete = napi::Class<ClassType>::Delete;
            napi::register_class(
                name,
                TypeID<ClassType>::get(),
                reinterpret_cast<GenericFunction>(New),
                reinterpret_cast<GenericFunction>(Delete),
                &napi::Class<ClassType>::prototype );
        }

        template<typename... ConstructorArgs, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& constructor(Policies... policies) const {
            return constructor(
                &internal::operator_new<ClassType, ConstructorArgs...>,
                policies...);
        }

        template<typename... Args, typename ReturnType, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& constructor(ReturnType(*factory)(Args...), Policies...) const {
            using namespace internal;

            auto invoke = &ConstructorInvoker<ClassType, Args...>::invoke;
            napi::register_class_constructor(
                TypeID<ClassType>::get(),
                sizeof...(Args),
                reinterpret_cast<GenericFunction>(invoke),
                reinterpret_cast<GenericFunction>(factory));
            return *this;
        }


        template<typename ReturnType, typename... Args, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& function(const char* methodName, ReturnType(ClassType::*memberFunction)(Args...), Policies...) const {
            using namespace internal;

            auto invoker = &MethodInvoker<decltype(memberFunction), ReturnType, ClassType*, Args...>::invoke;

            //typename WithPolicies<Policies...>::template ArgTypeList<ReturnType, AllowedRawPointer<ClassType>, Args...> args;
            napi::register_class_function(
                TypeID<ClassType>::get(),
                methodName,
                sizeof...(Args),
                reinterpret_cast<GenericFunction>(invoker),
                getContext(memberFunction),
                isPureVirtual<Policies...>::value);
            return *this;
        }
        template<typename ReturnType, typename... Args, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& function(const char* methodName, ReturnType(ClassType::*memberFunction)(Args...) const, Policies...) const {
            using namespace internal;

            auto invoker = &MethodInvoker<decltype(memberFunction), ReturnType, const ClassType*, Args...>::invoke;

            napi::register_class_function(
                TypeID<ClassType>::get(),
                methodName,
                sizeof...(Args),
                reinterpret_cast<GenericFunction>(invoker),
                getContext(memberFunction),
                isPureVirtual<Policies...>::value);
            return *this;
        }

        template<typename ReturnType, typename ThisType, typename... Args, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& function(const char* methodName, ReturnType(*function)(ThisType, Args...), Policies...) const {
            using namespace internal;

            auto invoker = &FunctionInvoker<decltype(function), ReturnType, ThisType, Args...>::invoke;
            napi::register_class_function(
                TypeID<ClassType>::get(),
                methodName,
                sizeof...(Args),
                reinterpret_cast<GenericFunction>(invoker),
                reinterpret_cast<GenericFunction>(function),
                isPureVirtual<Policies...>::value);
            return *this;
        }


        template<typename FieldType, typename = typename std::enable_if<!std::is_function<FieldType>::value>::type>
        EMSCRIPTEN_ALWAYS_INLINE const class_& property(const char* fieldName, const FieldType ClassType::*field) const {
            using namespace internal;

            auto getter = &MemberAccess<ClassType, FieldType>::template get<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(getter),
                getContext(field),
                0,
                0);
            return *this;
        }

        template<typename FieldType, typename = typename std::enable_if<!std::is_function<FieldType>::value>::type>
        EMSCRIPTEN_ALWAYS_INLINE const class_& property(const char* fieldName, FieldType ClassType::*field) const {
            using namespace internal;

            auto getter = &MemberAccess<ClassType, FieldType>::template get<ClassType>;
            auto setter = &MemberAccess<ClassType, FieldType>::template set<ClassType>;
            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(getter),
                getContext(field),
                reinterpret_cast<GenericFunction>(setter),
                getContext(field));
            return *this;
        }

        template<typename Getter>
        EMSCRIPTEN_ALWAYS_INLINE const class_& property(const char* fieldName, Getter getter) const {
            using namespace internal;

            typedef GetterPolicy<Getter> GP;
            auto gter = &GP::template get<ClassType>;

            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(gter),
                getContext(getter),
                0,
                0);
            return *this;
        }

        template<typename Getter, typename Setter>
        EMSCRIPTEN_ALWAYS_INLINE const class_& property(const char* fieldName, Getter getter, Setter setter) const {
            using namespace internal;
            typedef GetterPolicy<Getter> GP;
            typedef SetterPolicy<Setter> SP;

            auto gter = &GP::template get<ClassType>;
            auto ster = &SP::template set<ClassType>;

            napi::register_class_property(
                TypeID<ClassType>::get(),
                fieldName,
                reinterpret_cast<GenericFunction>(gter),
                getContext(getter),
                reinterpret_cast<GenericFunction>(ster),
                getContext(setter)
            );

            return *this;
        }


        template<typename ReturnType, typename... Args, typename... Policies>
        EMSCRIPTEN_ALWAYS_INLINE const class_& class_function(const char* methodName, ReturnType(*classMethod)(Args...), Policies...) const {
            using namespace internal;

            auto invoke = &Invoker<ReturnType, Args...>::invoke;
            napi::register_class_class_function(
                TypeID<ClassType>::get(),
                methodName,
                sizeof...(Args),
                reinterpret_cast<GenericFunction>(invoke),
                reinterpret_cast<GenericFunction>(classMethod));
            return *this;
        }

        template<typename FieldType>
        EMSCRIPTEN_ALWAYS_INLINE const class_& class_property(const char* name, const FieldType* field) const {
             using namespace internal;

             auto getter = &GlobalAccess<FieldType>::get;
             napi::register_class_class_property(
                 TypeID<ClassType>::get(),
                 name,
                 field,
                 reinterpret_cast<GenericFunction>(getter),
                 0);
             return *this;
        }
         
        template<typename FieldType>
        EMSCRIPTEN_ALWAYS_INLINE const class_& class_property(const char* name, FieldType* field) const {
            using namespace internal;
        
            auto getter = &GlobalAccess<FieldType>::get;
            auto setter = &GlobalAccess<FieldType>::set;
            napi::register_class_class_property(
                TypeID<ClassType>::get(),
                name,
                field,
                reinterpret_cast<GenericFunction>(getter),
                reinterpret_cast<GenericFunction>(setter));
            return *this;
        }

    };


}

#define EMSCRIPTEN_BINDINGS(name)                                       \
    static struct EmscriptenBindingInitializer_##name {                 \
        EmscriptenBindingInitializer_##name();                          \
    } EmscriptenBindingInitializer_##name##_instance;                   \
    EmscriptenBindingInitializer_##name::EmscriptenBindingInitializer_##name()

#endif // !_NODE_EMBIND_EMSCRIPTEN_BIND_H_