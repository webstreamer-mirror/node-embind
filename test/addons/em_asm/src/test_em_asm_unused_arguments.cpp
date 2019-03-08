// Copyright 2018 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <stdio.h>
#include <emscripten/emscripten.h>
namespace test_em_asm_unused_arguments{
	
int main() {
  int sum = EM_ASM_INT({
     return $0 + $2;
  }, 0, 1, 2);
  printf("0+2=%d\n", sum);
  return 0;
}


}