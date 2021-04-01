#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm("add %1,%0;"
  :"+r"(a)
  :"r"(b));
  return a;
}

int asm_popcnt(uint64_t x) {
  int s=0;
  asm("movl $0x0,%%eax;" //s
    "movl $0x0,%%ecx;"//  i  
    "L1: movq %%rbx, %%rdx;" //x移到临时变量
    "and $0x1,%%rdx;"//最后一位
    "test %%rdx,%%rdx;"//0/1
    "je L2;"
    "add $1,%%eax;"//s++
    "L2:shr $1,%%rbx;"//x>>1
    "inc %%ecx;"//i++
    "cmpl $0x40,%%ecx;"
    "jle L1;"//满足则继续循环
    : "=a" (s)
    : "b" (x)
    :"%ecx","%rdx","cc","memory"
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  void* start=dest;
  // asm volatile (
  // "test %%eax,%%eax;"//n!=0
  // "je L3;"
  // "mov %%edx,-0x18(%%ebp);"//18---dest
  // "mov %%ecx,-0x20(%%ebp);"//20---src
  // "mov %%eax,-0x28(%%ebp);"//28---n
  // "L0: mov -0x20(%%ebp),%%esi;"// 主循环
  // "movzbl (%%esi),%%ebx;" 
  // "mov -0x18(%%ebp),%%rsi;"
  // "mov %%bl,(%%esi);"
  // "subq  $0x1,-0x28(%%ebp);"//n--
  // "addq $0x1,-0x18(%%ebp);"//dest++
  // "addq  $0x1,-0x20(%%ebp);"//src++
  // "mov -0x28(%%ebp),%%esi;"
  // "test %%esi,%%esi;"//n!=0
  // "jne L0;"
  // "L3:"
  // :
  // :[dest]"d"(dest),[src]"c"(src),[number]"a"(n)
  // :"%esi","cc","memory","%ebx");
  asm volatile( 
    "1:lodsb;"
    "stosb;"
    "subq $0x1,%%rcx;"
    "testq %%rcx,%%rcx;"
    "jne 1b;"
    :
    :"r"(src),"r"(dest),"c"(n)
    :"memory");
  return start;
}

//寄存器 RBX、RBP、RDI、RSI、RSP、R12、R13、R14 和 R15 被视为非易失的，必须由使用它们的函数进行保存和还原。
//8个寄存器（RBX、RBP、RSP、R12、R13、R14 、 R15 及PC）的值，就保存在此变量中

int asm_setjmp(asm_jmp_buf env) {
  asm volatile(
      "push %%rbp;"
      "mov %%rsp,%%rbp;"
      "movq (%%rbp),%%rax;"
      "movq %%rax,(%%rdi);"//true rbp
      "leaq 0x10(%%rsp),%%rax;"
      "movq %%rax,0x8(%%rdi);"//true rsp
      "movq %%rbx,0x10(%%rdi);"//rbx
      "movq 0x8(%%rbp),%%rax;" //pc
      "movq %%rax,0x18(%%rdi);"
      "movq %%r12,0x20(%%rdi);"
      "movq %%r13,0x28(%%rdi);"
      "movq %%r14,0x30(%%rdi);"
      "movq %%r15,0x38(%%rdi);"
      "pop %%rbp;"
    :
    :"rdi"(env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm volatile(
    "movq 0x10(%%rdi),%%rbx;"
    "movq 0x20(%%rdi),%%r12;"
    "movq 0x28(%%rdi),%%r13;"
    "movq 0x30(%%rdi),%%r14;"
    "movq 0x38(%%rdi),%%r15;"
    "movq (%%rdi),%%rbp;"
    "movq 0x8(%%rdi),%%rsp;"
    "movq %%rsi,%%rax;"
    "movq 0x18(%%rdi),%%rdi;"
    "jmp *%%rdi;" //jmp to pc
    :
    :"rdi"(env),"rsi"(val)
  );
  
}
