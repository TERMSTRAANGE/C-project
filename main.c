#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/app.h"
#include "include/ui.h"
#include "include/storage.h"

int main() {
    AppData appData;
    initAppData(&appData);
    
    initDiscountModule(&appData);
    
    if (loadDataFromFile(&appData, "store_data.txt")) {
        printf("Successfully loaded data from store_data.txt\n");
    } else {
        printf("No existing data found, starting with empty database.\n");
    }
    
    printf("\n=== Welcome to the Store Management System ===\n");
    printf("Data will be automatically saved to 'store_data.txt'\n\n");
    mainMenu(&appData);
    
    if (saveDataToFile(&appData, "store_data.txt")) {
        printf("Data saved successfully to store_data.txt\n");
    } else {
        printf("Warning: Could not save data to file!\n");
    }
    freeAppData(&appData);
    
    printf("Thank you for using the Store Management System. Goodbye!\n");
    return 0;
}