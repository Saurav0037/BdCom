#include<stdio.h>
#include "input.c"

#define NAME_SIZE 20

int main(){
    int age;
    char name[NAME_SIZE];
    char gender;

    input_str("Please Enter your name: ",name,NAME_SIZE);
    input_int("Please Enter your age: ", &age);
    input_char("Please Enter you gender (M Or F): ",&gender);
    
    printf("You are %s\nyour age is %d\nand your gender is %c\n",name, age, gender);

}