#include <stdio.h>
#include <stdlib.h>
#include "app.h"
#include "admin.h"
#include "customer.h"

static void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif 
}

static void clearDelay(){
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

void customerMenu(AppData* appData){
    int choice;
    while(1){
        clearScreen();
        printf("Customer Menu\n");
        printf("1. List Products\n");
        printf("2. Order Products\n");
        printf("3. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice){
        case 1:
            listProducts(appData);
            clearDelay();
            break;
        case 2:
            listProducts(appData);
            orderProducts(appData);
            clearDelay();
            break;
        case 3:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            clearDelay();
            break;
        }
    }
}

void adminMenu(AppData* appData){
    int choice;
    while(1){
        clearScreen();
        printf("Administrator Menu\n");
        printf("1. List Products\n");
        printf("2. Add Product\n");
        printf("3. Edit Product \n");
        printf("4. Delete Product \n");
        printf("5. List Orders\n");
        printf("6. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice){
            case 1:
                listProducts(appData);
                clearDelay();
                break;
            case 2:
                addProduct(appData);
                clearDelay();
                break;
            case 3:{
                int productId;
                printf("Enter product ID to edit: ");
                scanf("%d", &productId);
                editProduct(appData, productId);
                clearDelay();
                break;
            }
            case 4:{
                listProducts(appData);
                int productId;
                printf("Enter product ID to delete: ");
                scanf("%d", &productId);
                deleteProduct(appData, productId);
                clearDelay();
                break;
            }
            case 5:
                listOrders(appData);
                clearDelay();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                clearDelay();
                break;
        }
    }
}

void mainMenu(AppData* appData){
    int choice;
    while(1){
        clearScreen();
        printf("Main Menu\n");
        printf("1. Customer\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice){
            case 1:
                customerMenu(appData);
                break;
            case 2:
                adminMenu(appData);
                break;
            case 3:
                printf("Exiting the program.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                clearDelay();
                break;
        }
    }
}
