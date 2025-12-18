#ifndef DISCOUNT_H
#define DISCOUNT_H

#include "app.h"

// Initialize the discount module with application data
void initDiscountModule(AppData* appData);

// Set customer status (No Discount, Standard, VIP)
int setCustomerStatus(int customerId, const char* status);

// Calculate discount based on customer status and subtotal
float calculateDiscount(const char* status, float subtotal);

#endif // DISCOUNT_H
