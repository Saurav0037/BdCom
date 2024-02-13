//#include "input.h"
#ifndef INPUT_H
#define INPUT_H
#include<stdio.h>
#include<string.h>

#define BUFFER_SIZE 100
#define ATTEMPT 3

int try_again(int attempt);
int input_str(char *message, char str[], int max_size);
int input_int(char *message, int *data);
int input_char(char *message, char *data);

int try_again(int attempt){
    if(attempt == 0){
        printf("Please try again later...\n");
        return 0;
    }
    else{
        printf("You have %d attempts left. try again.\n", attempt);
        return 1;
    }
    return 0;
}

int input_str(char *message, char str[], int max_size){
    char buffer[BUFFER_SIZE];
    int c, attempt = ATTEMPT;
    printf("%s\n",message);

str_input:
        scanf("%99[^\n]",buffer);
        while(c = getchar(), c != '\n' && c != EOF);
        if(strlen(buffer)>max_size){
            printf("Your name is too long.\n");
            if(try_again(attempt--))
                goto str_input;
        }
        else{
            strcpy(str,buffer);
            return 1; 
        }
    return 0;

}

int input_int(char *message, int *data){
    char ch;
    int temp, c, attempt = ATTEMPT;
    printf("%s\n",message);
int_input:
    
        if(scanf("%d%c",&temp,&ch)!=2 || ch!='\n'){
            printf("invalid integer input.\n");
            while(c = getchar(), c != '\n' && c != EOF);
            if(try_again(attempt--))
                goto int_input;
        }
        else{
            *data = temp;
            return 1;
        }
    return 0;
}

int input_char(char *message, char *data){
    char ch, temp;
    int c, attempt = ATTEMPT;
    printf("%s\n",message);
char_input:
    
        if(scanf("%c%c",&temp,&ch)!=2 || ch!='\n'){
            printf("invalid integer input.\n");
            while(c = getchar(), c != '\n' && c != EOF);
            if(try_again(attempt--))
                goto char_input;
        }
        else{
            *data = temp;
            return 1;
        }
    return 0;
}

#endif