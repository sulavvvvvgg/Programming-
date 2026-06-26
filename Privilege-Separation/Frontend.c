#include <stdio.h>

int main()
{
    char password[100];

    printf("Enter password: ");
    scanf("%99s", password);

    printf("You entered: %s\n", password);

    return 0;
}
