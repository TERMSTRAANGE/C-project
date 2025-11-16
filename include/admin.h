#ifndef ADMIN_H
#define ADMIN_H

#include "app.h"

void addProduct(AppData* appData);
void editProduct(AppData* appData, int productId);
void deleteProduct(AppData* appData, int productId);
void listOrders(AppData* appData);

#endif