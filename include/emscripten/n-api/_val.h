

#ifndef _NODE_EMBIND_EMSCRIPTEN_INTERNAL_VAL_H_
#define _NODE_EMBIND_EMSCRIPTEN_INTERNAL_VAL_H_
#include <node_api.h>
#include <assert.h>
namespace emscripten {
	namespace internal {

		struct val_t {

			enum valtype{
				undefined,
				null,
				boolean,
				number,
				string,
				symbol,
				object,
				function,
				external,
				bigint,

				array = 0x100,
				int8_array,
				uint8_array,
				uint8_clamped_array,
				int16_array,
				uint16_array,
				int32_array,
				uint32_array,
				float32_array,
				float64_array,
				bigint64_array,
				biguint64_array,

				none = 0xFFFFFFFF
			};




			napi_env   env_;
			napi_value value_;
			napi_ref   ref_;
			valtype    type_;

			val_t(napi_valuetype ntype) {
				assert(0);
			}
			val_t(napi_typedarray_type ntype) {
				assert(0);
			}

			template<typename T>
			explicit val_t(T&& value) 
				: env_(__NAPI_ENV__)
			{
				napi_value obj = napi::value<T&&>(env_).napi_cast(value);
			}

			val_t(valtype type = valtype::none) {

			}

			void ref();
			void unref();
		};
	}
}

#endif //!_NODE_EMBIND_EMSCRIPTEN_INTERNAL_VAL_H_