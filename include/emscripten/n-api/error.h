#ifndef _NODE_EMBIND_EMSCRIPTEN_NAPI_ERROR_H_
#define _NODE_EMBIND_EMSCRIPTEN_NAPI_ERROR_H_
#include <emscripten/n-api/common.h>

#define NODE_EMBIND_ERROR_NAPICALL     "10000"
#define NODE_EMBIND_ERROR_NAPICALL_MSG(status) _node_embind_napi_call_error(status)
#define NODE_EMBIND_ERROR_NAPICALL_CHECK(env, status) \
   if(status != napi_ok ) {\
     ::napi_throw_error(env, NODE_EMBIND_ERROR_NAPICALL, NODE_EMBIND_ERROR_NAPICALL_MSG(status));\
   }

#define NODE_EMBIND_ERROR_ARGC     "10100"
#define NODE_EMBIND_ERROR_ARGC_MSG "No overload function matched the parameter count."
#define NODE_EMBIND_ERROR_CTOR_ARGC_MSG "No constructor matched the parameter count."


inline static const char* _node_embind_napi_call_error(napi_status status) {
	switch (status)
	{
	    case napi_ok:                          return "napi call failed (napi_ok)."; 
		case napi_invalid_arg:                 return "napi call failed (napi_invalid_arg)."; 
		case napi_object_expected:             return "napi call failed (napi_object_expected)."; 
		case napi_string_expected:             return "napi call failed (napi_string_expected)."; 
		case napi_name_expected:               return "napi call failed (napi_name_expected)."; 
		case napi_function_expected:           return "napi call failed (napi_function_expected)."; 
		case napi_number_expected:             return "napi call failed (napi_number_expected)."; 
		case napi_boolean_expected:            return "napi call failed (napi_boolean_expected)."; 
		case napi_array_expected:              return "napi call failed (napi_array_expected)."; 
		case napi_generic_failure:             return "napi call failed (napi_generic_failure)."; 
		case napi_pending_exception:           return "napi call failed (napi_pending_exception)."; 
		case napi_cancelled:                   return "napi call failed (napi_cancelled)."; 
		case napi_escape_called_twice:         return "napi call failed (napi_escape_called_twice)."; 
		case napi_handle_scope_mismatch:       return "napi call failed (napi_handle_scope_mismatch)."; 
		case napi_callback_scope_mismatch:     return "napi call failed (napi_callback_scope_mismatch)."; 
		case napi_queue_full:                  return "napi call failed (napi_queue_full)."; 
		case napi_closing:                     return "napi call failed (napi_closing)."; 
		case napi_bigint_expected:             return "napi call failed (napi_bigint_expected).";
	default:
		std::string msg= std::string("napi call failed status=") + std::to_string(static_cast<int>(status));
		return msg.c_str();
		break;
	}

}



#endif // !#ifndef _NODE_EMBIND_EMSCRIPTEN_BIND_H_
