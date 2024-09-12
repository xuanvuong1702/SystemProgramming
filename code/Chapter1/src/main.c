
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "my_string.h"

void Question1()
{
    fprintf(stderr, "Hello ");
    fprintf(stdout, "It's a small ");
    fprintf(stderr, "World\n");
    fprintf(stdout, "place\n");
}

void Question2()
{
    char str1[] = "another";
    char *str2 = "another one";

    int sizeOfStr1 = sizeof(str1);
    int sizeOfStr2 = sizeof(str2);

    printf("Size of str1: %d\n", sizeOfStr1);
    printf("Size of str2: %d\n", sizeOfStr2);
}

void Question3()
{
    // what is the string in c?
    // asnwer: a string is a sequence of characters that is terminated by a null character '\0'
    // vietnamese: chuỗi là một chuỗi các ký tự kết thúc bởi ký tự null '\0'
}

void Question4_0()
{
    //what is the strcmp function in c?
    // answer: strcmp function is used to compare two strings. It returns 0 if the strings are equal, a positive value if the first string is greater than the second string, and a negative value if the first string is less than the second string.
    // vietnamese: hàm strcmp được sử dụng để so sánh hai chuỗi. Nó trả về 0 nếu hai chuỗi bằng nhau, một giá trị dương nếu chuỗi đầu tiên lớn hơn chuỗi thứ hai và một giá trị âm nếu chuỗi đầu tiên nhỏ hơn chuỗi thứ hai.
    char* str1 = "Hello";
    char* str2 = "Hello";
    int result = strcmp(str1, str2);

    int result2 = my_strcmp(str2, str1);

    printf("str1: Hello, str2: Hello, result: %d\n", result);

    char* str3 = "Hello";
    char* str4 = "World";

    result = strcmp(str3, str4);

    printf("str3: Hello, str4: World, result: %d\n", result);
}

void Question4_1()
{
    // what is the strcat function in c?
    // answer: strcat function is used to concatenate two strings. It appends a copy of the source string to the destination string.
    // vietnamese: hàm strcat được sử dụng để nối hai chuỗi. Nó thêm một bản sao của chuỗi nguồn vào chuỗi đích.
    char str1[20] = "Hello";
    char str2[20] = "World";

    strcat(str1, str2);

    printf("str1: %s\n", str1);
}

void Question4_2(){
    // what is the strcpy function in c?
    // answer: strcpy function is used to copy a string from the source to the destination. It copies the source string to the destination string including the null character.
    // vietnamese: hàm strcpy được sử dụng để sao chép một chuỗi từ nguồn đến đích. Nó sao chép chuỗi nguồn vào chuỗi đích bao gồm ký tự null.
    char str1[20] = "Hello";
    char str2[20];

    strcpy(str2, str1);

    printf("str2: %s\n", str2);

    //what happen if the destination string is smaller than the source string?
    // answer: if the destination string is smaller than the source string, the strcpy function will copy the source string to the destination string until it reaches the null character. It may cause a buffer overflow if the source string is larger than the destination string.
    // vietnamese: nếu chuỗi đích nhỏ hơn chuỗi nguồn, hàm strcpy sẽ sao chép chuỗi nguồn vào chuỗi đích cho đến khi nó đến ký tự null. 

    char str3[5] = "Hello";
    char str4[3];

    strcpy(str4, str3);

    printf("str4: %s\n", str4);


}

void Question12() {
    const char* filename = "example.txt";
    const char* str = "Hello, world!";
    printFileInChunks(filename, str);
}


int main()
{
    printf("Question 1\n");
    Question1();

    printf("\n\n");

    printf("Question 2\n");

    Question2();

    printf("\n\n");

    printf("Question 3\n");

    Question3();

    printf("\n\n");

    printf("Question 4.0\n");

    Question4_0();

    printf("\n\n");

    printf("Question 4.1\n");

    Question4_1();

    printf("\n\n");

    printf("Question 4.2\n");

    Question4_2();

    return 0;
}