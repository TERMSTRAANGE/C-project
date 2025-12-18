#pragma once
#include <stddef.h>

typedef struct{
    int productId;
    char name[50];
    float price;
    int stock;
} Product;

#define MAX_STATUS_LENGTH 16
#define STATUS_NO_DISCOUNT "No Discount"
#define STATUS_STANDARD "Standard"
#define STATUS_VIP "VIP"

typedef struct {
    int customerId;
    char name[50];
    char discountStatus[MAX_STATUS_LENGTH];
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

// Storage functions
int saveDataToFile(const AppData* appData, const char* filename);
int loadDataFromFile(AppData* appData, const char* filename);

// Discount functions
void initDiscountModule(AppData* appData);
int setCustomerStatus(int id, const char* status);
float calculateDiscount(const char* status, float subtotal);