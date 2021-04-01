#include "asm.h"
#include <assert.h>
#include <stdio.h>

// void *asm_memcpy1(void *dest, const void *src, size_t n) {
//   void* start=dest;
//   asm volatile (
//   "test %%rax,%%rax;"//n!=0
//   "je L3;"
//   "mov %%rdx,-0x18(%%rbp);"//18---dest
//   "mov %%rcx,-0x20(%%rbp);"//20---src
//   "mov %%rax,-0x28(%%rbp);"//28---n
//   "L0: mov -0x20(%%rbp),%%rsi;"// 主循环
//   "movzbl (%%rsi),%%ebx;" 
//   "mov -0x18(%%rbp),%%rsi;"
//   "mov %%bl,(%%rsi);"
//   "subq  $0x1,-0x28(%%rbp);"
//   "addq $0x1,-0x18(%%rbp);"
//   "addq  $0x1,-0x20(%%rbp);"
//   "mov -0x28(%%rbp),%%rsi;"
//   "test %%rsi,%%rsi;"
//   "jne L0;"
//   "L3:"
//   :
//   :[dest]"d"(dest),[src]"c"(src),[number]"a"(n));
//   return start;
// }
// int asm_setjmp1(asm_jmp_buf env) {
//   int re=0;
//   asm(
//       "cmpq $0,%%rax;"
//       "jle H;"
//       "xor %%rax,%%rax;"
//       "H:movq %%rax,%%rcx;"
//       "movq %%rsp,(%%rdi);"//true rbp
//       "leaq 0x10(%%rsp),%%rax;"
//       "movq %%rax,0x8(%%rdi);"//true rsp
//       "movq %%rbx,0x10(%%rdi);"//rbx
//       "movq 0x8(%%rbp),%%rax;"//pc
//       "movq %%rax,0x18(%%rdi);"
//       "movq %%r12,0x20(%%rdi);"
//       "movq %%r13,0x28(%%rdi);"
//       "movq %%r14,0x30(%%rdi);"
//       "movq %%r15,0x38(%%rdi);"
//     :"=c"(re)
//     :"rdi"(env),"c"(re)
//     :"%rax","cc","memory"
//   );
//   return re;
// }
// void asm_longjmp1(asm_jmp_buf env, int val) {
//   asm(
//     "movq 0x10(%%rdi),%%rbx;"
//     "movq 0x20(%%rdi),%%r12;"
//     "movq 0x28(%%rdi),%%r13;"
//     "movq 0x30(%%rdi),%%r14;"
//     "movq 0x38(%%rdi),%%r15;"
//     "movq (%%rdi),%%rbp;"
//     "movq 0x8(%%rdi),%%rsp;"
//     "movq 0x18(%%rdi),%%rdi;"
//     "jmp *%%rdi;"
//     :
//     :"rdi"(env),"a"(val)
//     :"cc","memory"
//   );
// }

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    // TODO: add more tests here.
    asm_longjmp(buf, 123);
    printf("666\n");
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
//   char*s="Golden Global View";
//   char d[20];
//   asm_memcpy1(d,s,4);
// return 0;
}
