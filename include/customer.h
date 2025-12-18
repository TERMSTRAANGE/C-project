#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdbool.h>
#include "app.h"


Customer* findCustomerByName(AppData* appData, const char* name);
Customer* createCustomer(AppData* appData, const char* name);
Customer* findOrCreateCustomer(AppData* appData, const char* name);
bool updateCustomerStatus(Customer* customer, const char* status);

#endif 