#include <stdio.h>

int main(){
    int choice;
    while(1){
        printf("Main Menu\n");
        printf("1. Customer\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Product Search.\n");
            printf("Will be implemented in later release.\n");
            break;
        case 2:
            printf("Administrator Login.\n");
            printf("Will be implemented in later release.\n");
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