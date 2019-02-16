#ifndef _NODE_EMBIND_EMSCRIPTEN_INTERNAL_EMBIND_H_
#define _NODE_EMBIND_EMSCRIPTEN_INTERNAL_EMBIND_H_

namespace emscripten {
	namespace internal {
		struct _embind {
			inline static void register_function(
				const char* name,
				unsigned argCount,
				const TYPEID argTypes[],
				const char* signature,
				GenericFunction invoker,
				GenericFunction function);

			inline static void register_class(
				const char* name,
				TYPEID classType,
				TYPEID pointerType,
				TYPEID constPointerType,
				GenericFunction New);

			inline static void register_class_constructor(
				TYPEID classType,
				unsigned argCount,
				const TYPEID argTypes[],
				const char* invokerSignature,
				GenericFunction invoker,
				GenericFunction constructor);

			inline static void register_class_property(
				TYPEID classType,
				const char* fieldName,
				GenericFunction getter,
				void* getterContext,
				GenericFunction setter,
				void* setterContext);

			inline static void register_class_function(
				TYPEID classType,
				const char* methodName,
				unsigned argCount,
				const TYPEID argTypes[],
				GenericFunction invoker,
				void* context,
				unsigned isPureVirtual);

			inline static void register_class_class_function(
				TYPEID classType,
				const char* methodName,
				unsigned argCount,
				const TYPEID argTypes[],
				const char* invokerSignature,
				GenericFunction invoker,
				GenericFunction method);
			inline static void register_class_class_property(
				TYPEID classType,
				const char* fieldName,
				const void* fieldContext,
				GenericFunction getter,
				GenericFunction setter);

			inline static void register_enum(
				TYPEID enumType,
				const char* name,
				size_t size,
				bool isSigned,
				GenericFunction New,
				napi_callback getter);

			inline static void register_enum_value(
				TYPEID enumType,
				const char* valueName,
				GenericEnumValue value);

			inline static void register_constant(
				const char* name,
				TYPEID constantType,
				void*  function,
				void*  const_value);

		};
	}
}

#endif //!_NODE_EMBIND_EMSCRIPTEN_INTERNAL_EMBIND_H_