//
//  sierracall.h
//  sierracall
//
//  Created by card on 10/12/16.

#ifndef sierracall_h
#define sierracall_h

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int64_t sierracall(uint64_t numofargs, uint64_t callnum, ...)
{
    va_list ap;
    uint64_t *argarray = malloc(sizeof(uint64_t) * numofargs);
    int64_t returnval = -1;
    uint64_t j = 1;
    
    va_start(ap, callnum);
    argarray[0] = callnum;
    for (int i = 1; i < numofargs; i++) {
        argarray[i] = va_arg(ap, uint64_t);
    }
    
    asm volatile(
                 "movq %[argarray], %%r11\n\t"
                 "movq %[numofargs], %%r12\n\t"
                 "movq %[j], %%r13\n\t"
                 
                 "movq -8(%%r11, %%r13, 8), %%rax\n\t" //sierracall num goes in %rax
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%rdi\n\t"
                 "cmpq %%r13, %%r12\n\t" // have we popped enough args for the sierracall?
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%rsi\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%rdx\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%rcx\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%r8\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "incq %%r13\n\t"
                 "movq -8(%%r11, %%r13, 8), %%r9\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t"
                 
                 "stackPop: \n\t"
                 "incq %%r13\n\t"
                 "pushq -8(%%r11, %%r13, 8)\n\t"
                 "cmpq %%r13, %%r12\n\t"
                 "je donePopping\n\t" // if
                 "jmp stackPop\n\t" // else
                 
                 "donePopping:\n\t"
                 "syscall\n\t"
                 "movq %%rax, %[returnval]"
                : [j] "+m" (j), [returnval] "+m" (returnval), [callnum] "+m" (callnum), [numofargs] "+m" (numofargs), [argarray] "+m" (argarray)
                );
    
    va_end(ap);
    
    if (returnval < 0)
    {
        errno = -1 * returnval;
    }
    
    return returnval;
}

#endif /* sierracall_h */
