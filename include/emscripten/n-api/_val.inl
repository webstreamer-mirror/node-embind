#include <emscripten/wire.h>
#include <emscripten/val.h>
#include <node_api.h>

#include <iostream>
namespace emscripten {
	namespace internal {

		struct _EM_DESTRUCTORS {

		};
		struct _EM_METHOD_CALLER
		{

		};
		void _emval_register_symbol(const char*);



		inline void _emval_incref(EM_VAL value) {
			if (value && value->ref) {
				uint32_t result;
				napi_reference_ref(value->env, value->ref, &result);
			}
		}

		inline void _emval_decref(EM_VAL value) {
			if (value && value->ref) {
				uint32_t result;
				napi_reference_unref(value->env, value->ref, &result);
			}
		}

		void _emval_run_destructors(EM_DESTRUCTORS handle);

		inline EM_VAL _emval_new_array() {
			EM_VAL v = new _EM_VAL(TypeEx::_array);
			napi_create_array(v->env, &v->value);
			return v;
		}

		inline EM_VAL _emval_new_object() {
			EM_VAL v = new _EM_VAL(napi_object);
			napi_create_object(v->env, &v->value);
			v->type = napi_object;
			napi_create_reference(v->env, v->value, 0, &v->ref);
			return v;
		}
		
		inline EM_VAL _emval_new_cstring(const char* str) {
			EM_VAL v = new _EM_VAL(napi_string);
			napi_create_string_latin1(v->env, str, NAPI_AUTO_LENGTH, &v->value);
			return v;
		}

		EM_VAL _emval_take_value(TYPEID type, EM_VAR_ARGS argv);

		EM_VAL _emval_new(
			EM_VAL value,
			unsigned argCount,
			const TYPEID argTypes[],
			EM_VAR_ARGS argv);

		inline EM_VAL _emval_get_global(const char* name) {
			napi_value global;
			EM_VAL obj = new _EM_VAL(napi_object);
			napi_get_global(obj->env, &global);
			if (name) {
				napi_value key;
				napi_create_string_utf8(obj->env, name, NAPI_AUTO_LENGTH, &key);
				napi_get_property(obj->env, global, key, &obj->value);
			}
			else {
				obj->value = global;
			}
			return obj;
		}
		EM_VAL _emval_get_module_property(const char* name);
		EM_VAL _emval_get_property(EM_VAL object, EM_VAL key);
		
		inline void _emval_set_property(EM_VAL object, EM_VAL key, EM_VAL value) {
			napi_set_property(object->env, object->value, key->value, value->value);
		}

		EM_GENERIC_WIRE_TYPE _emval_as(EM_VAL value, TYPEID returnType, EM_DESTRUCTORS* destructors);

		bool _emval_equals(EM_VAL first, EM_VAL second);
		bool _emval_strictly_equals(EM_VAL first, EM_VAL second);
		bool _emval_greater_than(EM_VAL first, EM_VAL second);
		bool _emval_less_than(EM_VAL first, EM_VAL second);
		bool _emval_not(EM_VAL object);

		EM_VAL _emval_call(
			EM_VAL value,
			unsigned argCount,
			const TYPEID argTypes[],
			EM_VAR_ARGS argv);

		// DO NOT call this more than once per signature. It will
		// leak generated function objects!
		EM_METHOD_CALLER _emval_get_method_caller(
			unsigned argCount, // including return value
			const TYPEID argTypes[]);
		EM_GENERIC_WIRE_TYPE _emval_call_method(
			EM_METHOD_CALLER caller,
			EM_VAL handle,
			const char* methodName,
			EM_DESTRUCTORS* destructors,
			EM_VAR_ARGS argv);
		void _emval_call_void_method(
			EM_METHOD_CALLER caller,
			EM_VAL handle,
			const char* methodName,
			EM_VAR_ARGS argv);
		EM_VAL _emval_typeof(EM_VAL value);
		bool _emval_instanceof(EM_VAL object, EM_VAL constructor);
		bool _emval_in(EM_VAL item, EM_VAL object);
		bool _emval_delete(EM_VAL object, EM_VAL property);
		bool _emval_throw(EM_VAL object);



		/////////////
		inline static EM_VAL _emval_undefined() {
			EM_VAL v = new _EM_VAL(napi_undefined);
			napi_get_undefined(v->env, &v->value);
			return v;		
		}

		inline static EM_VAL _emval_null() {
			EM_VAL v = new _EM_VAL(napi_null);
			napi_get_null(v->env, &v->value);
			return v;
		}


		inline static EM_VAL _emval_true() {
			EM_VAL v = new _EM_VAL(napi_boolean);
			napi_get_boolean(v->env, true, &v->value);
			return v;

		}
		inline static EM_VAL _emval_false() {
			EM_VAL v = new _EM_VAL(napi_boolean);
			napi_get_boolean(v->env, false, &v->value);
		}



		inline static napi_valuetype _emval_napi_typeof(EM_VAL v) {
			napi_valuetype result;
			napi_typeof(v->env, v->value, &result);
			return result;
		}


		inline static bool _emval_get_boolean(EM_VAL v) {
			bool result;
			napi_get_value_bool(v->env, v->value, &result);
				return result;
		}


	}
}