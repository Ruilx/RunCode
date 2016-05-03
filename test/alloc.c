/***************************************
** >MODULE NAME< Module
** Made By Ruilx
****************************************
** Title:          >PUT TITLE HERE<
** Time:           >Year</>Month</>Day<
** Author:         Ruilx
** Organization:   GT-Soft Studio
** License:        GPL
**
** Target:         >PUT TARGET HERE<
** Version:        0.1.0
****************************************
** Copyright(C) GT-Soft Studio
**                  All Rights Reserved.
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

#define SIZE1 512
#define SIZE2 1024

int main(){
        char *a = (char *)malloc(sizeof(char)* SIZE1 * SIZE2);
        printf("Here's a's address: %p, already mallocced %ubytes=%.0fk\n", a, SIZE1*SIZE2, ((double)SIZE1*SIZE2/1024.0));
        Sleep(100);
        char *b = (char *)malloc(sizeof(char)* SIZE1 * SIZE2);
        printf("Here's a's address: %p, already mallocced %ubytes=%.0fk\n", a, SIZE1*SIZE2, ((double)SIZE1*SIZE2/1024.0));
        Sleep(100);
        char aa[SIZE1][SIZE2];
        printf("Here's aa's address: %p, already took %ubytes=%.0fk\n", aa, SIZE1*SIZE2, ((double)SIZE1*SIZE2/1024.0));
        Sleep(100);
        char bb[SIZE1][SIZE2];
        printf("Here's bb's address: %p, already took %ubytes=%.0fk\n", bb, SIZE1*SIZE2, ((double)SIZE1*SIZE2/1024.0));
        Sleep(100);
	free(a);
	printf("Free a.\n");
	Sleep(100);
	free(b);
	printf("Free b.\n");
	Sleep(100);
	return 65588;
}
