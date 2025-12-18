#ifndef UI_H
#define UI_H

#include "app.h"
#include "customer.h"

// menu
void mainMenu(AppData* app);
void adminMenu(AppData* app);
void customerMenu(AppData* app);

// output functions
void displayProducts(AppData* app);
void displayCustomers(AppData* app);
void displayOrders(AppData* app);

// customer functions
void listProducts(AppData* app);
void orderProducts(AppData* app);

#endif
