#include <stdio.h>
#include "app.h"
#include "customer.h"

void addProduct(AppData* appData){
    if(appData->products.count >= appData->products.capacity){
        increaseAppDataCapacity(appData, 0);
    }
    Product newProduct;
    printf("Enter product name: ");
    scanf("%s", newProduct.name);
    printf("Enter product price: ");
    scanf("%f", &newProduct.price);
    printf("Enter product stock: ");
    scanf("%d", &newProduct.stock);
    appData->products.items[appData->products.count++] = newProduct;
}

void editProduct(AppData* appData, int productId){
    if(productId < 0 || productId >= appData->products.count){
        printf("Invalid product ID.\n");
        return;
    }
    Product* product = &appData->products.items[productId];
    printf("Editing product: %s\n", product->name);
    printf("Enter new product name: ");
    scanf("%s", product->name);
    printf("Enter new product price: ");
    scanf("%f", &product->price);
    printf("Enter new product stock: ");
    scanf("%d", &product->stock);
}

void deleteProduct(AppData* appData, int productId){
    if(productId < 0 || productId >= appData->products.count){
        printf("Invalid product ID.\n");
        return;
    }
    for(int i = 0; i < appData->orders.count; i++){
        if(appData->orders.items[i].productId == productId){
            appData->orders.items[i].productId = -1;
        }
    }
    for(int i = productId; i < appData->products.count - 1; i++){
        appData->products.items[i] = appData->products.items[i + 1];
        appData->products.items[i].productId = i;
    }
    appData->products.count--;
}

void listOrders(AppData* appData){
    printf("Order List:\n");
    for(int i = 0; i < appData->orders.count; i++){
        Order* order = &appData->orders.items[i];
        Customer* customer = &appData->customers.items[order->customerId];
        if(order->productId < 0){
            char deletedProductName[] = "Deleted Product";
            printf("Order ID: %d, Customer: %s, Product: %s, Quantity: %d, Discount: %.2f\n",
                   order->orderId, customer->name, deletedProductName, order->quantity, order->discount);
            continue;
        }
        Product* product = &appData->products.items[order->productId];
        printf("Order ID: %d, Customer: %s, Product: %s, Quantity: %d, Discount: %.2f\n",
               order->orderId, customer->name, product->name, order->quantity, order->discount);
    }
}