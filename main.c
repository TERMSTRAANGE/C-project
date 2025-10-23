#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // #

struct Product{
    char name[50];
    float price;
    int stock;
};

struct Order{
    int orderId;
    int customerId;
    int productId;
    int quantity;
    float discount;
};

struct Customer{
    int customerId;
    char name[50];
};

struct Product products[10];
struct Order orders[100];
struct Customer customers[10];
int productCount = 0;
int customerCount = 0;
int orderCount = 0;

const char* ADMIN_PASSWORD = "admin123"; // # admin password

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

// # Case-insensitive substring check utility
int containsCaseInsensitive(const char* haystack, const char* needle) { // #
    if (!haystack || !needle) return 0; 
    size_t nlen = strlen(needle); 
    if (nlen == 0) return 1; 
    for (const char* h = haystack; *h; ++h) { 
        size_t i = 0; 
        while (needle[i] && h[i] && tolower((unsigned char)h[i]) == tolower((unsigned char)needle[i])) i++; // #
        if (i == nlen) return 1; 
    } 
    return 0; 
} 

void addProduct(){
    if (productCount >= 10) {
        printf("Product list is full.");
        return;
    }
    struct Product newProduct;
    printf("Enter product name: ");
    scanf("%49s", newProduct.name);
    printf("Enter product price: ");
    scanf("%f", &newProduct.price);
    printf("Enter product stock: ");
    scanf("%d", &newProduct.stock);
    products[productCount] = newProduct;
    productCount++;
    printf("Product added successfully.\n");
}

void listProducts(){
    if (productCount == 0){
        printf("No products available.\n");
        return;
    }
    printf("Product List:\n");
    for (int i = 0; i < productCount; i++){
        printf("%d. Name: %s, Price: %.2f, Stock: %d\n", i + 1, products[i].name, products[i].price, products[i].stock);
    }
}

void searchProduct(){
    if (productCount == 0){
        printf("No products available.\n");
        return;
    }
    char search[50];
    printf("Enter product name to search: ");
    scanf("%49s", search);
    int found = 0;
    for (int i = 0; i < productCount; i++){
        if (strcmp(products[i].name, search) == 0){
            printf("Product found: Name: %s, Price: %.2f, Stock: %d\n", products[i].name, products[i].price, products[i].stock);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Product not found.\n");
    }
}

void editProduct(){
    printf("Select product to edit:\n");
    listProducts();
    int choice;
    printf("Enter product number: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > productCount){
        printf("Invalid product number.\n");
        return;
    }
    struct Product *prod = &products[choice - 1];
    printf("Choose field to edit:\n");
    printf("1. Name\n");
    printf("2. Price\n");
    printf("3. Stock\n");
    int field;
    printf("Enter your choice: ");
    scanf("%d", &field);
    switch (field){
    case 1:
        printf("Enter new name: ");
        scanf("%49s", prod->name);
        break;
    case 2:
        printf("Enter new price: ");
        scanf("%f", &prod->price);
        break;
    case 3:
        printf("Enter new stock: ");
        scanf("%d", &prod->stock);
        break;
    default:
        printf("Invalid choice.\n");
        return;
    }
    printf("Product updated successfully.\n");
}

void deleteProduct(){
    printf("Select product to delete:\n");
    listProducts();
    int choice;
    printf("Enter product number: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > productCount){
        printf("Invalid product number.\n");
        return;
    }
    for (int i = choice - 1; i < productCount - 1; i++){
        products[i] = products[i + 1];
    }
    productCount--;
    printf("Product deleted successfully.\n");
}

int customerCheck(char* name){
    for (int i = 0; i < customerCount; i++){
        if (strcmp(customers[i].name, name) == 0){
            return customers[i].customerId;
        }
    }
    struct Customer newCustomer;
    newCustomer.customerId = customerCount + 1;
    strcpy(newCustomer.name, name);
    customers[customerCount] = newCustomer;
    customerCount++;
    return newCustomer.customerId;
}

void listOrders(){
    if (orderCount == 0){
        printf("No orders placed.\n");
        return;
    }
    printf("Order List:\n");
    for (int i = 0; i < orderCount; i++){
        struct Order ord = orders[i];
        struct Customer cust = customers[ord.customerId - 1];
        struct Product prod = products[ord.productId - 1];
        printf("Order ID: %d, Customer: %s, Product: %s, Quantity: %d, Discount: %.2f\n",
               ord.orderId, cust.name, prod.name, ord.quantity, ord.discount);
    }
}

int customerMenu(){
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
            listProducts();
            clearDelay();
            break;
        case 2:
            if (productCount == 0){
                printf("No products available to order.\n");
                clearDelay();
                break;
            }

            listProducts();

            // # search (case-insensitive, partial)
            {
                char search[50]; 
                printf("\nSearch product by name (partial, case-insensitive). Enter * to keep full list: "); 
                scanf("%49s", search); 
                if (strcmp(search, "*") != 0) { 
                    int any = 0; 
                    printf("\nFiltered Results:\n"); 
                    for (int i = 0; i < productCount; i++) { 
                        if (containsCaseInsensitive(products[i].name, search)) { 
                            printf("%d. Name: %s, Price: %.2f, Stock: %d\n", i + 1, products[i].name, products[i].price, products[i].stock); 
                            any = 1; 
                        } 
                    } 
                    if (!any) { 
                        printf("No matching products.\n"); 
                        clearDelay(); 
                        break; 
                    } 
                } 
            } 

            printf("Select product to order:\n");
            int prodChoice, quantity;
            printf("Enter product number: ");
            scanf("%d", &prodChoice);
            if (prodChoice < 1 || prodChoice > productCount){
                printf("Invalid product number.\n");
                clearDelay();
                break;
            }
            printf("Enter quantity: ");
            scanf("%d", &quantity);
            if (quantity > products[prodChoice - 1].stock){
                printf("Insufficient stock.\n");
                clearDelay();
                break;
            }
            printf("Enter your name: ");
            char custName[50];
            scanf("%49s", custName);
            int custId = customerCheck(custName);
            products[prodChoice - 1].stock -= quantity;
            struct Order newOrder;
            newOrder.orderId = orderCount + 1;
            newOrder.customerId = custId;
            newOrder.productId = prodChoice;
            newOrder.quantity = quantity;
            newOrder.discount = 0.0;
            orders[orderCount] = newOrder;
            orderCount++;
            printf("Order placed successfully.\n");
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

// # admin password verification before pening admin menu
int verifyAdminPassword() { 
    char input[64]; 
    printf("Enter admin password: "); 
    scanf("%63s", input); 
    if (strcmp(input, ADMIN_PASSWORD) == 0) return 1; 
    printf("Incorrect password.\n"); 
    clearDelay(); 
    return 0; 
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
        printf("5. List Orders\n");
        printf("6. Exit to Main Menu\n");
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
            editProduct();
            clearDelay();
            break;
        case 4:
            deleteProduct();
            clearDelay();
            break;
        case 5:
            listOrders();
            clearDelay();
            break;
        case 6:
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
            if (verifyAdminPassword()) { adminMenu(); } // #
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
