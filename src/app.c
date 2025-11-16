#include <stdlib.h>
#include "app.h"

static void initProductList(ProductList* list){
    list->count = 0;
    list->capacity = 10;
    list->items = malloc(list->capacity * sizeof(Product));
}

static void initOrderList(OrderList* list){
    list->count = 0;
    list->capacity = 50;
    list->items = malloc(list->capacity * sizeof(Order));
}

static void initCustomerList(CustomerList* list){
    list->count = 0;
    list->capacity = 10;
    list->items = malloc(list->capacity * sizeof(Customer));
}

void initAppData(AppData* appData){
    initProductList(&appData->products);
    initOrderList(&appData->orders);
    initCustomerList(&appData->customers);
}

void increaseAppDataCapacity(AppData* appData, int dataType){
    switch (dataType){
        case 0: // Products
            appData->products.capacity *= 2;
            appData->products.items = realloc(appData->products.items, appData->products.capacity * sizeof(Product));
            break;
        case 1: // Orders
            appData->orders.capacity *= 2;
            appData->orders.items = realloc(appData->orders.items, appData->orders.capacity * sizeof(Order));
            break;
        case 2: // Customers
            appData->customers.capacity *= 2;
            appData->customers.items = realloc(appData->customers.items, appData->customers.capacity * sizeof(Customer));
            break;
    }
}

void freeAppData(AppData* appData){
    free(appData->products.items);
    free(appData->orders.items);
    free(appData->customers.items);
}