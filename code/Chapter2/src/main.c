
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


void TestPork()
{
   int answer = 84 >> 1;
    printf("Answer: %d", answer);
    fork();
}


int main()
{
    TestPork();
    return 0;
}