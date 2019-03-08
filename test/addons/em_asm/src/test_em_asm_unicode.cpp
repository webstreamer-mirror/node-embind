// Copyright 2018 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <emscripten/emscripten.h>

namespace test_em_asm_unicode{
	
int main() {
#if 0
  EM_ASM( out("hello world…") );
#endif
  return 0;
}

}
