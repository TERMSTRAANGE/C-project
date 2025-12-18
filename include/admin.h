#ifndef ADMIN_H
#define ADMIN_H

#include "app.h"
#ifdef __cplusplus
extern "C" {
#endif
void addProduct(AppData* appData);
void editProduct(AppData* appData, int productId);
void deleteProduct(AppData* appData, int productId);
void listOrders(AppData* appData);
#ifdef __cplusplus
}
#endif
#endif 