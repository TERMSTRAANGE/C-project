# Documentation

## Project Structure

```
C-project/
├── main.c              # Entry point of the application
├── CMakeLists.txt      # Build configuration
├── include/            # Header files
│   ├── admin.h         
│   ├── app.h           
│   ├── customer.h      
│   ├── storage.h       
│   └── ui.h            
├── src/                # Source files
│   ├── admin.c         # Administrator functionality
│   ├── app.c           # Application data management
│   ├── customer.c      # Customer functionality
│   ├── storage.c       # Storage functionality
│   └── ui.c            # User interface
└── docs/               # Documentation
```

## Data Storage Structure

Core data structures are saved in dynamic lists:
- **ProductList**: Manages products
  ```c
  typedef struct{
      Product* items;
      size_t count;
      size_t capacity;
  } ProductList;
  ```
- **OrderList**: Manages orders
  ```c
  typedef struct{
      Order* items;
      size_t count;
      size_t capacity;
  } OrderList;
  ```
- **CustomerList**: Manages Customers
  ```c
  typedef struct{
      Customer* items;
      size_t count;
      size_t capacity;
  } CustomerList;
  ```

Count refers to the current amount of objects saved in these lists, whereas capacity shows maximum number of items that can be saved before resizing.

These structures are saved within the AppData container:
```c
typedef struct {
    ProductList products;
    Orderlist orders;
    CustomerList customers;
}
```
[`initAppData()`](../src/app.c): initializes data structures with default capacities

[`increaseAppDataCapacity()`](../src/app.c): increases capacity of data structures

[`freeAppData()`](../src/app.c): cleans up allocated memory