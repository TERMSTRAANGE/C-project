#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "app.h"

static void trimNewline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int saveDataToFile(const AppData* appData, const char* filename) {
    if (!appData || !filename) {
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return 0;
    }

    // save products
    if (fprintf(file, "PRODUCTS %zu\n", appData->products.count) < 0) {
        fclose(file);
        return 0;
    }
    
    for (size_t i = 0; i < appData->products.count; i++) {
        const Product* p = &appData->products.items[i];
        if (fprintf(file, "%d|%s|%.2f|%d\n", 
                   p->productId, p->name, p->price, p->stock) < 0) {
            fclose(file);
            return 0;
        }
    }

    // save customers
    if (fprintf(file, "CUSTOMERS %zu\n", appData->customers.count) < 0) {
        fclose(file);
        return 0;
    }
    
    for (size_t i = 0; i < appData->customers.count; i++) {
        const Customer* c = &appData->customers.items[i];
        if (fprintf(file, "%d|%s|%s\n", 
                   c->customerId, c->name, c->discountStatus) < 0) {
            fclose(file);
            return 0;
        }
    }

    // save orders
    if (fprintf(file, "ORDERS %zu\n", appData->orders.count) < 0) {
        fclose(file);
        return 0;
    }
    
    for (size_t i = 0; i < appData->orders.count; i++) {
        const Order* o = &appData->orders.items[i];
        if (fprintf(file, "%d|%d|%d|%d|%.2f\n", 
                   o->orderId, o->customerId, o->productId, 
                   o->quantity, o->discount) < 0) {
            fclose(file);
            return 0;
        }
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
        return 0;
    }
    
    return 1;
}

int loadDataFromFile(AppData* appData, const char* filename) {
    if (!appData || !filename) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return 0;
    }

    char line[1024];
    char section[32];
    int loading = 0; // 0=none, 1=products, 2=customers, 3=orders
    int line_num = 0;
    int count = 0;
    int result = 1;

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        trimNewline(line);
        
        if (line[0] == '\0') continue;

        // check for section headers
        if (sscanf(line, "%31s %d", section, &count) == 2) {
            if (strcmp(section, "PRODUCTS") == 0) {
                loading = 1;
                continue;
            } else if (strcmp(section, "CUSTOMERS") == 0) {
                loading = 2;
                continue;
            } else if (strcmp(section, "ORDERS") == 0) {
                loading = 3;
                continue;
            }
        }
        
        switch (loading) {
            case 1: { 
                Product p = {0};
                if (sscanf(line, "%d|%49[^|]|%f|%d", 
                          &p.productId, p.name, &p.price, &p.stock) == 4) {
                    if (appData->products.count >= appData->products.capacity) {
                        increaseAppDataCapacity(appData, 0);  // 0 is the type for products
                    }
                    appData->products.items[appData->products.count++] = p;
                } else {
                    fprintf(stderr, "Invalid product format on line %d\n", line_num);
                }
                break;
            }
            case 2: { // customers
                Customer c = {0};
                if (sscanf(line, "%d|%49[^|]|%15s", 
                          &c.customerId, c.name, c.discountStatus) == 3) {
                    if (strcmp(c.discountStatus, STATUS_NO_DISCOUNT) != 0 &&
                        strcmp(c.discountStatus, STATUS_STANDARD) != 0 &&
                        strcmp(c.discountStatus, STATUS_VIP) != 0) {
                        strncpy(c.discountStatus, STATUS_NO_DISCOUNT, MAX_STATUS_LENGTH - 1);
                        c.discountStatus[MAX_STATUS_LENGTH - 1] = '\0';
                        fprintf(stderr, "Warning: Invalid discount status on line %d, set to '%s'\n", 
                                line_num, STATUS_NO_DISCOUNT);
                    }
                    if (appData->customers.count >= appData->customers.capacity) {
                        increaseAppDataCapacity(appData, 2);  // 2 is the type for customers
                    }
                    appData->customers.items[appData->customers.count++] = c;
                } else {
                    fprintf(stderr, "Invalid customer format on line %d\n", line_num);
                }
                break;
            }
            case 3: { // orders
                Order o = {0};
                if (sscanf(line, "%d|%d|%d|%d|%f", 
                          &o.orderId, &o.customerId, &o.productId, 
                          &o.quantity, &o.discount) == 5) {
                    if (o.quantity <= 0) {
                        fprintf(stderr, "Warning: Invalid quantity %d on line %d, order skipped\n", 
                                o.quantity, line_num);
                        continue;
                    }
                    if (appData->orders.count >= appData->orders.capacity) {
                        increaseAppDataCapacity(appData, 1);  // 1 is the type for orders
                    }
                    appData->orders.items[appData->orders.count++] = o;
                } else {
                    fprintf(stderr, "Invalid order format on line %d\n", line_num);
                }
                break;
            }
            default:
                fprintf(stderr, "Unexpected data outside of section on line %d\n", line_num);
                break;
        }
    }
cleanup:
    if (fclose(file) != 0) {
        perror("Error closing file");
        result = 0;
    }
    
    return result;
}