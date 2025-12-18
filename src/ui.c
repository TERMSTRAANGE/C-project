#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/app.h"
#include "../include/admin.h"
#include "../include/customer.h"
#include "../include/ui.h"
#include "../include/storage.h"
#include "../include/discount.h"

// forward declarations
void updateCustomerStatusUI(AppData* appData);
void placeOrder(AppData* appData);
void checkCustomerStatus(const AppData* appData);

static void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif 
}

static void clearDelay(){
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

void updateCustomerStatusUI(AppData* appData) {
    clearScreen();
    printf("=== Update Customer Status ===\n\n");
    
    if (appData->customers.count == 0) {
        printf("No customers found.\n");
        clearDelay();
        return;
    }
    
    printf("ID  %-30s %-15s\n", "Name", "Status");
    printf("----------------------------------------\n");
    for (int i = 0; i < appData->customers.count; i++) {
        printf("%-3d %-30s %-15s\n", 
               appData->customers.items[i].customerId,
               appData->customers.items[i].name,
               appData->customers.items[i].discountStatus);
    }
    
    int customerId;
    printf("\nEnter customer ID to update (or -1 to cancel): ");
    scanf("%d", &customerId);
    
    if (customerId == -1) return;
    
    // find customer
    int found = 0;
    for (int i = 0; i < appData->customers.count; i++) {
        if (appData->customers.items[i].customerId == customerId) {
            found = 1;
            printf("\nCurrent status: %s\n", appData->customers.items[i].discountStatus);
            printf("\nSelect new status:\n");
            printf("1. No Discount\n");
            printf("2. Standard (10%% discount)\n");
            printf("3. VIP (20%% discount)\n");
            printf("Enter choice (1-3): ");
            
            int choice;
            scanf("%d", &choice);
            
            const char* newStatus = NULL;
            switch (choice) {
                case 1: newStatus = STATUS_NO_DISCOUNT; break;
                case 2: newStatus = STATUS_STANDARD; break;
                case 3: newStatus = STATUS_VIP; break;
                default:
                    printf("Invalid choice. No changes made.\n");
                    clearDelay();
                    return;
            }
            
            // update status
            for (int i = 0; i < appData->customers.count; i++) {
                if (appData->customers.items[i].customerId == customerId) {
                    strncpy(appData->customers.items[i].discountStatus, newStatus, MAX_STATUS_LENGTH - 1);
                    appData->customers.items[i].discountStatus[MAX_STATUS_LENGTH - 1] = '\0';
                    break;
                }
            }
            
            // update through discount module
            if (setCustomerStatus(customerId, newStatus)) {
                printf("\nStatus updated successfully to %s\n", newStatus);
                if (!saveDataToFile(appData, "store_data.txt")) {
                    printf("Warning: Could not save changes to file!\n");
                }
            } else {
                printf("\nFailed to update status. Invalid customer ID.\n");
            }
            break;
        }
    }
    if (!found) {
        printf("\nCustomer not found.\n");
    }
    clearDelay();
}

// customer functions
void listProducts(AppData* appData) {
    clearScreen();
    printf("=== Product List ===\n");
    if (appData->products.count == 0) {
        printf("No products available.\n");
        return;
    }
    for (size_t i = 0; i < appData->products.count; i++) {
        printf("%zu. %s - $%.2f (Stock: %d)\n", 
               i, 
               appData->products.items[i].name,
               appData->products.items[i].price,
               appData->products.items[i].stock);
    }
}

void placeOrder(AppData* appData) {
    if (!appData) {
        printf("Error: Invalid application data.\n");
        clearDelay();
        return;
    }

    clearScreen();
    printf("=== Place an Order ===\n\n");
    
    if (appData->products.count == 0) {
        printf("No products available to order.\n");
        clearDelay();
        return;
    }
    
    listProducts(appData);
    
    // customer information
    char customerName[50] = {0};
    printf("\nEnter your name: ");
    while (getchar() != '\n'); 
    
    if (fgets(customerName, sizeof(customerName), stdin) == NULL) {
        printf("\nError reading input.\n");
        clearDelay();
        return;
    }
    customerName[strcspn(customerName, "\n")] = '\0'; // Remove newline
    
    if (strlen(customerName) == 0) {
        printf("\nError: Customer name cannot be empty.\n");
        clearDelay();
        return;
    }
    
    Customer* customer = findOrCreateCustomer(appData, customerName);
    if (!customer) {
        printf("\nError: Could not create or find customer.\n");
        clearDelay();
        return;
    }
    
    printf("\n%s, %s! Your status: %s\n",
           customer == findCustomerByName(appData, customerName) ? "Welcome back" : "New customer created",
           customer->name,
           customer->discountStatus);
    
    int productId, quantity;
    printf("\nEnter product ID to order: ");
    scanf("%d", &productId);
    
    if (productId < 0 || productId >= (int)appData->products.count) {
        printf("Invalid product ID.\n");
        clearDelay();
        return;
    }
    
    Product* product = &appData->products.items[productId];
    
    printf("Enter quantity (available: %d): ", product->stock);
    scanf("%d", &quantity);
    
    if (quantity <= 0) {
        printf("Invalid quantity.\n");
        clearDelay();
        return;
    }
    
    if (product->stock < quantity) {
        printf("Not enough stock available.\n");
        clearDelay();
        return;
    }
    
    // detail calculation
    float subtotal = product->price * quantity;
    float discount = calculateDiscount(customer->discountStatus, subtotal);
    float total = subtotal - discount;
    
    product->stock -= quantity;
    
    if (appData->orders.count >= appData->orders.capacity) {
        increaseAppDataCapacity(appData, 1); // 1 is for orders
    }
    
    Order* order = &appData->orders.items[appData->orders.count++];
    order->orderId = appData->orders.count;
    order->customerId = customer->customerId;
    order->productId = productId;
    order->quantity = quantity;
    order->discount = discount;
    
    if (!saveDataToFile(appData, "store_data.txt")) {
        printf("\nWarning: Could not save order to file!\n");
    }
    
    printf("\n=== Order Confirmation ===\n");
    printf("Customer: %s\n", customer->name);
    printf("Status: %s\n", customer->discountStatus);
    printf("Product: %s\n", product->name);
    printf("Quantity: %d\n", quantity);
    printf("Price per unit: $%.2f\n", product->price);
    printf("Subtotal: $%.2f\n", subtotal);
    printf("Discount: $%.2f (%s)\n", discount, customer->discountStatus);
    printf("Total: $%.2f\n", total);
    printf("\nThank you for your order!\n");
    clearDelay();
}

void checkCustomerStatus(const AppData* appData) {
    clearScreen();
    printf("=== Check Customer Status ===\n\n");
    
    if (appData->customers.count == 0) {
        printf("No customers found.\n");
        clearDelay();
        return;
    }
    
    char customerName[50];
    printf("Enter your name: ");
    while (getchar() != '\n'); 
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0'; // Remove newline
    
    const Customer* customer = NULL;
    for (int i = 0; i < appData->customers.count; i++) {
        if (strcasecmp(appData->customers.items[i].name, customerName) == 0) {
            customer = &appData->customers.items[i];
            break;
        }
    }
    
    if (!customer) {
        printf("\nCustomer not found.\n");
        clearDelay();
        return;
    }
    
    // Display customer info
    printf("\n=== Customer Information ===\n");
    printf("Name: %s\n", customer->name);
    printf("Status: %s\n", customer->discountStatus);
    
    // Find and display orders
    printf("\n=== Order History ===\n");
    int orderCount = 0;
    for (int i = 0; i < appData->orders.count; i++) {
        if (appData->orders.items[i].customerId == customer->customerId) {
            orderCount++;
            Order* order = &appData->orders.items[i];
            Product* product = &appData->products.items[order->productId];
            printf("\nOrder #%d\n", order->orderId);
            if(order->productId < 0){
                printf("Product: Deleted Product\n");
                printf("Quantity: %d\n", order->quantity);
                printf("Price: $0.00\n");
                printf("Discount: $%.2f\n", order->discount);
                printf("Total: $%.2f\n", 
                      (0.00 * order->quantity) - order->discount);
                continue;
            }
            printf("Product: %s\n", product->name);
            printf("Quantity: %d\n", order->quantity);
            printf("Price: $%.2f\n", product->price);
            printf("Discount: $%.2f\n", order->discount);
            printf("Total: $%.2f\n", 
                  (product->price * order->quantity) - order->discount);
        }
    }
    
    if (orderCount == 0) {
        printf("No orders found for this customer.\n");
    }
    
    clearDelay();
}

void customerMenu(AppData* appData) {
    int choice;
    while(1) {
        clearScreen();
        printf("=== Customer Menu ===\n\n");
        printf("1. List Products\n");
        printf("2. Place an Order\n");
        printf("3. Check My Status & Orders\n");
        printf("4. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                listProducts(appData);
                clearDelay();
                break;
            case 2:
                placeOrder(appData);
                break;
            case 3:
                checkCustomerStatus(appData);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                clearDelay();
                break;
        }
    }
}

void adminMenu(AppData* appData) {
    int choice;
    while(1) {
        clearScreen();
        printf("=== Administrator Menu ===\n\n");
        printf("1. List Products\n");
        printf("2. Add Product\n");
        printf("3. Edit Product\n");
        printf("4. Delete Product\n");
        printf("5. List All Orders\n");
        printf("6. Manage Customer Status\n");
        printf("7. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer on invalid input
            while (getchar() != '\n');
            choice = -1; // Force default case
        }
        
        switch (choice) {
            case 1:
                listProducts(appData);
                clearDelay();
                break;
                
            case 2:
                addProduct(appData);
                printf("\nProduct added successfully!\n");
                if (!saveDataToFile(appData, "store_data.txt")) {
                    printf("Warning: Could not save changes to file!\n");
                }
                clearDelay();
                break;
                
            case 3: {
                listProducts(appData);
                if (appData->products.count > 0) {
                    int productId;
                    printf("\nEnter product ID to edit: ");
                    scanf("%d", &productId);
                    
                    if (productId >= 0 && productId < (int)appData->products.count) {
                        editProduct(appData, productId);
                        printf("\nProduct updated successfully!\n");
                        if (!saveDataToFile(appData, "store_data.txt")) {
                            printf("Warning: Could not save changes to file!\n");
                        }
                    } else {
                        printf("\nInvalid product ID.\n");
                    }
                }
                clearDelay();
                break;
            }
                
            case 4: {
                listProducts(appData);
                if (appData->products.count > 0) {
                    int productId;
                    printf("\nEnter product ID to delete: ");
                    scanf("%d", &productId);
                    
                    if (productId >= 0 && productId < (int)appData->products.count) {
                        deleteProduct(appData, productId);
                        printf("\nProduct deleted successfully!\n");
                        if (!saveDataToFile(appData, "store_data.txt")) {
                            printf("Warning: Could not save changes to file!\n");
                        }
                    } else {
                        printf("\nInvalid product ID.\n");
                    }
                }
                clearDelay();
                break;
            }
                
            case 5:
                listOrders(appData);
                clearDelay();
                break;
                
            case 6:
                updateCustomerStatusUI(appData);
                break;
                
            case 7:
                return;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
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
