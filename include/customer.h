#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "app.h"

extern const char* STATUS_NO_DISCOUNT;
extern const char* STATUS_STANDARD;
extern const char* STATUS_VIP;

Customer* findCustomerByName(AppData* appData, const char* name);
Customer* createCustomer(AppData* appData, const char* name);
Customer* findOrCreateCustomer(AppData* appData, const char* name);
bool updateCustomerStatus(Customer* customer, const char* status);

#endif 