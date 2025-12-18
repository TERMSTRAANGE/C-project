#include <string.h>
#include <stdio.h>
#include "app.h"

static AppData* globalAppData = NULL;

void initDiscountModule(AppData* appData) {
    globalAppData = appData;
}

float calculateDiscount(const char* status, float subtotal) {
    if (strcmp(status, STATUS_STANDARD) == 0) {
        return subtotal * 0.10f; // 10% discount
    } else if (strcmp(status, STATUS_VIP) == 0) {
        return subtotal * 0.20f; // 20% discount
    }
    return 0.0f; // no discount
}

int setCustomerStatus(int customerId, const char* status) {
    if (!globalAppData) {
        fprintf(stderr, "Error: Discount module not initialized\n");
        return 0;
    }
    // validate status
    if (strcmp(status, STATUS_NO_DISCOUNT) != 0 &&
        strcmp(status, STATUS_STANDARD) != 0 &&
        strcmp(status, STATUS_VIP) != 0) {
        return 0; // invalid
    }
    
    for (int i = 0; i < globalAppData->customers.count; i++) {
        if (globalAppData->customers.items[i].customerId == customerId) {
            strncpy(globalAppData->customers.items[i].discountStatus, status, MAX_STATUS_LENGTH - 1);
            globalAppData->customers.items[i].discountStatus[MAX_STATUS_LENGTH - 1] = '\0';
            return 1; //success
        }
    }  
    return 0; // returns when customer not found
}
