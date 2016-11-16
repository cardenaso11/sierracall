//
//  sierracall.h
//  sierracall
//
//  Created by card on 10/12/16.
//  Licensed under the MIT License:
/*The MIT License (MIT)
 Copyright (c) 2016 card
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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
