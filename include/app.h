#pragma once
#include <stddef.h>

typedef struct{
    int productId;
    char name[50];
    float price;
    int stock;
} Product;

typedef struct{
    int customerId;
    char name[50];
} Customer;

typedef struct{
    int orderId;
    int customerId;
    int productId;
    int quantity;
    float discount;
} Order;

typedef struct{
    Product* items;
    size_t count;
    size_t capacity;
} ProductList;

typedef struct{
    Order* items;
    size_t count;
    size_t capacity;
} OrderList;

typedef struct{
    Customer* items;
    size_t count;
    size_t capacity;
} CustomerList;

typedef struct{ 
    ProductList products;
    OrderList orders;
    CustomerList customers;
} AppData;

void initAppData(AppData* appData);
void increaseAppDataCapacity(AppData* appData, int dataType);
void freeAppData(AppData* appData);