#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODUCTS 10
#define MAX_LENGTH 50

char itemList[MAX_PRODUCTS][MAX_LENGTH];
int productCount = 0;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearDelay() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

void addProduct(){
    if (productCount >= MAX_PRODUCTS) {
        printf("Product list is full.\n");
        return;
    }
    printf("Enter product name: ");
    scanf("%s", itemList[productCount]);
    productCount++;
    printf("Product added successfully.\n");
}

void listProducts(){
    if (productCount == 0){
        printf("No products available.\n");
        return;
    }
    printf("Available Products:\n");
    for (int i = 0; i < productCount; i++){
        printf("%d. %s\n", i + 1, itemList[i]);
    }
}

void searchProduct(){
    char search[MAX_LENGTH];
    printf("Enter search term: ");
    scanf("%s", search);
    int found = 0;
    for(int i = 0; i < productCount; i++){
        if (strcmp(itemList[i], search) == 0){
            printf("Product found: %s\n", itemList[i]);
            return;
        }
    }
    if (!found){
        printf("Product not found.\n");
    }
}

int customerMenu(){
    int choice;
    while(1){
        clearScreen();
        printf("Customer Menu\n");
        printf("1. List Products\n");
        printf("2. Search Product\n");
        printf("3. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice){
        case 1:
            listProducts();
        clearDelay();
            break;
        case 2:
            searchProduct();
        clearDelay();
            break;
        case 3:
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        clearDelay();
            break;
        }
    }
}

int adminMenu(){
    int choice;
    while(1){
        clearScreen();
        printf("Administrator Menu\n");
        printf("1. List Products\n");
        printf("2. Add Product\n");
        printf("3. Edit Product \n");
        printf("4. Delete Product \n");
        printf("5. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice){
        case 1:
            listProducts();
        clearDelay();
            break;
        case 2:
            addProduct();
        clearDelay();
            break;
        case 3:
            printf("Edit Product.\n");
            printf("Will be implemented in later release.\n");
        clearDelay();
            break;
        case 4:
            printf("Delete Product.\n");
            printf("Will be implemented in later release.\n");
        clearDelay();
            break;
        case 5:
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        clearDelay();
            break;
        }
    }
}

int mainMenu(){
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
            customerMenu();
            break;
        case 2:
            adminMenu();
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

int main(){
    return mainMenu();
}