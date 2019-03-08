/*
 * Copyright 2017 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifndef __em_asm_h__
#define __em_asm_h__
#include <emscripten/bind.h>
#include <emscripten/n-api/jscript.h>
#include <emscripten/n-api/jfunction.h>

#define _embindEM_ASM(code, ...)  \
    emscripten::internal::napi::JScript( #code ).Call( __VA_ARGS__ );

#define _embindEM_ASM_INT(code, ...) \
    emscripten::internal::napi::value<int>::napivalue(*emscripten::internal::napi::VM::context(),\
    emscripten::internal::napi::JScript(#code).Call( __VA_ARGS__ )\
    )

#define _embindEM_ASM_DOUBLE(code, ...) \
    emscripten::napi::value<double>(emscripten::internal::cur_env()).native_cast(\
    emscripten::internal::napi::JScript(#code).Call( __VA_ARGS__ )\
    )



// Note: If the code block in the EM_ASM() family of functions below contains a comma,
// then wrap the whole code block inside parentheses (). See test/em_asm/binding.cpp
// for example code snippets.

// Runs the given JavaScript code on the calling thread (synchronously), and returns no value back.
#define EM_ASM  _embindEM_ASM

// Runs the given JavaScript code on the calling thread (synchronously), and returns an integer back.
#define EM_ASM_INT _embindEM_ASM_INT

// Runs the given JavaScript code on the calling thread (synchronously), and returns a double back.
#define EM_ASM_DOUBLE _embindEM_ASM_DOUBLE


#endif // __em_asm_h__
