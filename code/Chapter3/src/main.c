
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    void *top_of_heap = sbrk(0);
    malloc(16384);
    void *top_of_heap2 = sbrk(0);
    printf("The top of heap went from %p to %p \n", top_of_heap, top_of_heap2);
    return 0;
}