#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app.h"
#include "customer.h"

Customer* findCustomerByName(AppData* appData, const char* name) {
    if (!appData || !name) return NULL;
    
    for (size_t i = 0; i < appData->customers.count; i++) {
        if (strcasecmp(appData->customers.items[i].name, name) == 0) {
            return &appData->customers.items[i];
        }
    }
    return NULL;
}

Customer* createCustomer(AppData* appData, const char* name) {
    if (!appData || !name || strlen(name) == 0) {
        return NULL;
    }
    
    // Check if customer already exists
    if (findCustomerByName(appData, name)) {
        return NULL;
    }
    
    // Resize array if needed
    if (appData->customers.count >= appData->customers.capacity) {
        increaseAppDataCapacity(appData, 2);
    }
    
    // Initialize new customer
    Customer* customer = &appData->customers.items[appData->customers.count];
    strncpy(customer->name, name, sizeof(customer->name) - 1);
    customer->name[sizeof(customer->name) - 1] = '\0';
    customer->customerId = appData->customers.count;
    strncpy(customer->discountStatus, STATUS_NO_DISCOUNT, MAX_STATUS_LENGTH - 1);
    customer->discountStatus[MAX_STATUS_LENGTH - 1] = '\0';
    
    appData->customers.count++;
    return customer;
}

Customer* findOrCreateCustomer(AppData* appData, const char* name) {
    if (!appData || !name) return NULL;
    
    Customer* customer = findCustomerByName(appData, name);
    if (!customer) {
        customer = createCustomer(appData, name);
    }
    return customer;
}

bool updateCustomerStatus(Customer* customer, const char* status) {
    if (!customer || !status) return false;
    
    // Validate status
    if (strcmp(status, STATUS_NO_DISCOUNT) != 0 &&
        strcmp(status, STATUS_STANDARD) != 0 &&
        strcmp(status, STATUS_VIP) != 0) {
        return false;
    }
    
    strncpy(customer->discountStatus, status, MAX_STATUS_LENGTH - 1);
    customer->discountStatus[MAX_STATUS_LENGTH - 1] = '\0';
    return true;
}
