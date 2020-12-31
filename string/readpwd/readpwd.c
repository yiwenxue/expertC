#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   
int main()
{
    char *password; // password string pointer
    printf("System func.\n");
    password = getpass("Enter Password: "); // get a password
    printf("%s\n",password); // this is just for conformation
                             // that password stored successfully
    printf("Custom func.\nEnter Password: ");

    char ch;
    char pwd[255]; 
    int i = 0;
    while (1) {
        ch = getc(stdin);

        if (ch == '\t' || ch == '\n'){
            pwd[i] = '\0';
            break;
        } else if (ch == '\b'){
            if (i > 0){
                i--;
                printf("\b \b");
            }
        }else{
            pwd[i++] = ch;
            printf("* \b");
        }
    }
    printf("h4ck3r: %s\n", pwd);
    return 0;
}
