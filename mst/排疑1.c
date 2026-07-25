// 1.	下面有段有问题的 ANSIC 代码，如果仍然维持代码的基本结构，
// 即仍然有两个函数，请问，如何修改，才能保证程序正确执行？
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* to allocate memory for a string variable */
void GetMemory ( char * p, unsigned int size )
{
    p=(char*)malloc( size);
    return;
} /* end of GetMemory */

// 1.
// void GetMemory ( char ** p, unsigned int size )
// {
    //     *p=(char*)malloc( size);
    //     return;
    // } /* end of GetMemory */

// 2.
// void GetMemory (unsigned int size )
// {
//     return p=(char*)malloc( size);
// } /* end of GetMemory */


void main (void)
{
    char *str=NULL;
    GetMemory (str, 100);        /* to get memory */
    strcpy (str,"Anyka is a hi-tech company."); /* set the string */
    printf(str);
    return;
} /* end of main */
