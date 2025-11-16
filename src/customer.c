#include <stdio.h>
#include <string.h>
#include "app.h"

void listProducts(AppData* appData){
    printf("Product List:\n");
    for(int i = 0; i < appData->products.count; i++){
        Product* product = &appData->products.items[i];
        printf("ID: %d, Name: %s, Price: %.2f, Stock: %d\n", i, product->name, product->price, product->stock);
    }
}

int customerCheck(AppData* appData, const char* custName){
    for(int i = 0; i < appData->customers.count; i++){
        if(strcmp(appData->customers.items[i].name, custName) == 0){
            return i;
        }
    }
    if(appData->customers.count >= appData->customers.capacity){
        increaseAppDataCapacity(appData, 2);
    }
    Customer newCustomer;
    strncpy(newCustomer.name, custName, sizeof(newCustomer.name) - 1);
    newCustomer.name[sizeof(newCustomer.name) - 1] = '\0';
    appData->customers.items[appData->customers.count] = newCustomer;
    return appData->customers.count++;
}

void orderProducts(AppData* appData){
    int prodChoice, quantity;
    printf("Enter product ID: ");
    scanf("%d", &prodChoice);
    if (prodChoice < 0 || prodChoice >= appData->products.count){
        printf("Invalid product ID.\n");
        return;
    }
    printf("Enter quantity: ");
    scanf("%d", &quantity);
    if (quantity > appData->products.items[prodChoice].stock){
        printf("Insufficient stock.\n");
        return;
    }
    printf("Enter your name: ");
    char custName[50];
    scanf("%49s", custName);
    int custId = customerCheck(appData, custName);
    appData->products.items[prodChoice].stock -= quantity;
    if(appData->orders.count >= appData->orders.capacity){
        increaseAppDataCapacity(appData, 1);
    }
    Order newOrder;
    newOrder.orderId = appData->orders.count + 1;
    newOrder.customerId = custId;
    newOrder.productId = prodChoice;
    newOrder.quantity = quantity;
    newOrder.discount = 0.0;
    appData->orders.items[appData->orders.count++] = newOrder;
    printf("Order placed successfully.\n");
}
