#include <stdio.h>

void mainMenu(){
    printf("Main Menu\n");
    printf("1. Customer\n");
    printf("2. Administrator\n");
    printf("3. Exit\n");
}

int main(){
    int choice;
    while(1){
        mainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Customer selected.\n");
            break;
        case 2:
            printf("Administrator selected.\n");
            break;
        case 3:
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
}