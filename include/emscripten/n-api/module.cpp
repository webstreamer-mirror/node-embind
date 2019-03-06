
#ifndef _IN_EMSCRIPTEN_NODE_EMBIND_FILE_
#error emscripten/n-api/module.cpp can only be include in <emscripten/node-embind>
#endif

#include <emscripten/bind.h>



NS_NAPI_BEGIN

bool Module::EMBIND_STD_STRING_IS_UTF8 = false;
bool Module::GARBAGE_CLEANING = true;


napi_value Module::Init(napi_env env, napi_value exports) {
	module_t& m = node_module();

	Preprocess(m.functions);

	auto prop = make_napi_property_table(m.functions);

	prop.push_back({ "EMBIND_STD_STRING_IS_UTF8", nullptr,nullptr,
		EMBIND_STD_STRING_IS_UTF8_getter, EMBIND_STD_STRING_IS_UTF8_setter,
		nullptr,napi_default,nullptr });

	napi_define_properties(env, exports, prop.size(), prop.data());

	std::map<TYPEID, class_t*>& classes = m.classes;
	for (std::map<TYPEID, class_t*>::iterator it = classes.begin();
		it != classes.end(); it++) {
		class_t* c = it->second;

		Register(c, env, exports);

	}



	return exports;
}


//====================================
//       Registers
//====================================

void register_function(
	const char* name,
	unsigned argc,
	GenericFunction invoker,
	GenericFunction function)
{
	module_t& m = node_module();
	function_t* f = new function_t();
	f->name = name;
	f->argc = argc;
	typedef napi_value(*Fn)(const function_t*, const context_t&);

	f->invoke = (Fn)invoker;

	f->function = function;
	m.functions.push_back(f);
}


void register_class(
	const char* name,
	TYPEID classType,
	GenericFunction New,
	GenericFunction Delete,
    class_t** pprototype,
    class_t::SubType subtype)
{

	module_t& m = node_module();
	class_t* c = new class_t();
	c->name = name;

	typedef napi_value (*Fn)(napi_env, napi_callback_info);
	c->New = (Fn)(New);
	c->Delete = (Fn)(Delete);
    c->subtype = subtype;
	m.classes[classType] = c;
    *pprototype = c;
	
}

void register_class_constructor(
	TYPEID classType,
	unsigned argc,
	GenericFunction invoker,
	GenericFunction function)
{
	module_t& m = node_module();
	constructor_t* ctor = new constructor_t();
	ctor->argc = argc;

	typedef class_ptr(*Fn) (const constructor_t*, const context_t&);
	ctor->invoke = (Fn)(invoker);
	ctor->function = function;

	m.classes[classType]->ctors.push_back(ctor);
}



void register_class_property(
	TYPEID classType,
	const char* fieldName,
	GenericFunction getter,
	void* getterContext,
	GenericFunction setter,
	void* setterContext,
    GenericFunction object_setter)

{
	module_t& m = node_module();
	property_t* prop = new property_t;
	prop->name = fieldName;

	typedef napi_value(*Setter)(const property_t*, const context_t&);
	prop->setter = (Setter)setter;
	prop->setter_context = setterContext;

	typedef napi_value(*Getter)(const property_t*, const context_t&);

	prop->getter = (Getter)getter;
	prop->getter_context = getterContext;

    typedef void(*OSetter)(const property_t*, const context_t&,void*, napi_value);
    prop->object_setter = (OSetter)object_setter;


	m.classes[classType]->property.push_back(prop);
}



void register_class_function(
    TYPEID classType,
    const char* methodName,
    unsigned argc,
    GenericFunction invoker,
    void* context,
    unsigned isPureVirtual)
{
    module_t& m = node_module();
    function_t* fn = new function_t();
    fn->argc = argc;
    fn->name = methodName;
    typedef napi_value(*Fn)(const function_t*, const context_t&);
    fn->invoke = (Fn)(invoker);
    fn->function = context;
    m.classes[classType]->function.push_back(fn);
}

void register_class_class_function(
    TYPEID classType,
    const char* methodName,
    unsigned argc,
    GenericFunction invoker,
    GenericFunction method)
{
    module_t& m = node_module();
    function_t* fn = new function_t();
    fn->argc = argc;
    fn->name = methodName;

    typedef napi_value(*Fn)(const function_t*, const context_t&);
    fn->invoke = (Fn)invoker;
    fn->function = method;
    fn->attributes = napi_static;
    m.classes[classType]->function.push_back(fn);
}

void register_class_class_property(
    TYPEID classType,
    const char* fieldName,
    const void* fieldContext,
    GenericFunction getter,
    GenericFunction setter)
{
    module_t& m = node_module();
    property_t* prop = new property_t;
    prop->name = fieldName;

    typedef napi_value(*Setter)(const property_t*, const context_t&);
    prop->setter = (Setter)setter;
    prop->setter_context = (void*)fieldContext;

    typedef napi_value(*Getter)(const property_t*, const context_t&);

    prop->getter = (Getter)getter;
    prop->getter_context = (void*)fieldContext;
    prop->attributes = napi_static;
    m.classes[classType]->property.push_back(prop);
}
NS_NAPI_END




NAPI_MODULE(NODE_GYP_MODULE_NAME, emscripten::internal::napi::Module::Init);

