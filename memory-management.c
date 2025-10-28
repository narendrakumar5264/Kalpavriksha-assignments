#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50
#define MAX_PRODUCTS 100

typedef struct {
    int id;
    char name[NAME_LENGTH];
    float price;
    int quantity;
} Product;

void showMenu() {
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

void printProduct(Product item) {
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           item.id, item.name, item.price, item.quantity);
}

void addProduct(Product **inventory, int *totalProducts) {
    if (*totalProducts >= MAX_PRODUCTS) {
        printf("Inventory limit reached! Cannot add more products.\n");
        return;
    }

    *inventory = realloc(*inventory, (*totalProducts + 1) * sizeof(Product));
    if (*inventory == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }

    Product *newItem = &((*inventory)[*totalProducts]);
    printf("Enter new product details:\n");

    printf("Product ID: ");
    if (scanf("%d", &newItem->id) != 1 || newItem->id <= 0) {
        printf("Invalid Product ID! Must be a positive integer.\n");
        return;
    }
    getchar();

    printf("Product Name: ");
    fgets(newItem->name, NAME_LENGTH, stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;

    printf("Product Price: ");
    if (scanf("%f", &newItem->price) != 1 || newItem->price <= 0) {
        printf("Invalid price! Must be positive.\n");
        return;
    }

    printf("Product Quantity: ");
    if (scanf("%d", &newItem->quantity) != 1 || newItem->quantity < 0) {
        printf("Invalid quantity! Must be 0 or more.\n");
        return;
    }

    (*totalProducts)++;
    printf("Product added successfully!\n");
}

void viewAllProducts(Product *inventory, int totalProducts) {
    if (totalProducts == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int index = 0; index < totalProducts; index++) {
        printProduct(inventory[index]);
    }
}

void updateProductQuantity(Product *inventory, int totalProducts) {
    int searchID, found = 0;
    printf("Enter Product ID to update quantity: ");
    if (scanf("%d", &searchID) != 1 || searchID <= 0) {
        printf("Invalid Product ID!\n");
        return;
    }

    for (int index = 0; index < totalProducts; index++) {
        if (inventory[index].id == searchID) {
            printf("Enter new Quantity: ");
            if (scanf("%d", &inventory[index].quantity) != 1 || inventory[index].quantity < 0) {
                printf("Invalid quantity!\n");
                return;
            }
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Product not found!\n");
    }
}

void searchProductByID(Product *inventory, int totalProducts) {
    int searchID, found = 0;
    printf("Enter Product ID to search: ");
    if (scanf("%d", &searchID) != 1 || searchID <= 0) {
        printf("Invalid Product ID!\n");
        return;
    }

    for (int index = 0; index < totalProducts; index++) {
        if (inventory[index].id == searchID) {
            printf("Product Found: ");
            printProduct(inventory[index]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Product not found!\n");
    }
}

void searchProductByName(Product *inventory, int totalProducts) {
    char searchName[NAME_LENGTH];
    int found = 0;
    printf("Enter name to search (partial allowed): ");
    getchar();
    fgets(searchName, NAME_LENGTH, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    printf("Products Found:\n");
    for (int index = 0; index < totalProducts; index++) {
        if (strstr(inventory[index].name, searchName) != NULL) {
            printProduct(inventory[index]);
            found = 1;
        }
    }

    if (!found) {
        printf("No products matched the search.\n");
    }
}

void searchProductByPriceRange(Product *inventory, int totalProducts) {
    float minPrice, maxPrice;
    int found = 0;

    printf("Enter minimum price: ");
    if (scanf("%f", &minPrice) != 1 || minPrice < 0) {
        printf("Invalid minimum price!\n");
        return;
    }

    printf("Enter maximum price: ");
    if (scanf("%f", &maxPrice) != 1 || maxPrice < minPrice) {
        printf("Invalid maximum price! It must be >= minimum price.\n");
        return;
    }

    printf("Products in Price Range (%.2f - %.2f):\n", minPrice, maxPrice);
    for (int index = 0; index < totalProducts; index++) {
        if (inventory[index].price >= minPrice && inventory[index].price <= maxPrice) {
            printProduct(inventory[index]);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found in this price range.\n");
    }
}

void deleteProductByID(Product **inventory, int *totalProducts) {
    int deleteID, found = 0;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &deleteID) != 1 || deleteID <= 0) {
        printf("Invalid Product ID!\n");
        return;
    }

    for (int position = 0; position < *totalProducts; position++) {
        if ((*inventory)[position].id == deleteID) {
            found = 1;
            for (int shift = position; shift < *totalProducts - 1; shift++) {
                (*inventory)[shift] = (*inventory)[shift + 1];
            }

            *inventory = realloc(*inventory, (*totalProducts - 1) * sizeof(Product));
            (*totalProducts)--;

            printf("Product deleted successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Product not found!\n");
    }
}

int main() {
    int totalProducts;
    printf("Enter initial number of products (0-%d): ", MAX_PRODUCTS);
    if (scanf("%d", &totalProducts) != 1 || totalProducts < 0 || totalProducts > MAX_PRODUCTS) {
        printf("Invalid product count! Must be between 0 and %d.\n", MAX_PRODUCTS);
        return 1;
    }

    Product *inventory = (Product *)calloc(totalProducts, sizeof(Product));
    if (inventory == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int index = 0; index < totalProducts; index++) {
        printf("Enter details for product %d:\n", index + 1);

        printf("Product ID: ");
        if (scanf("%d", &inventory[index].id) != 1 || inventory[index].id <= 0) {
            printf("Invalid Product ID!\n");
            return 1;
        }
        getchar();

        printf("Product Name: ");
        fgets(inventory[index].name, NAME_LENGTH, stdin);
        inventory[index].name[strcspn(inventory[index].name, "\n")] = 0;

        printf("Product Price: ");
        if (scanf("%f", &inventory[index].price) != 1 || inventory[index].price <= 0) {
            printf("Invalid price!\n");
            return 1;
        }

        printf("Product Quantity: ");
        if (scanf("%d", &inventory[index].quantity) != 1 || inventory[index].quantity < 0) {
            printf("Invalid quantity!\n");
            return 1;
        }
    }

    int choice;
    while (1) {
        showMenu();
        printf("Enter your choice (1-8): ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8) {
            printf("Invalid choice! Must be between 1 and 8.\n");
            while (getchar() != '\n'); 
            continue;
        }
        getchar();

        if (choice == 1) {
            addProduct(&inventory, &totalProducts);
        } else if (choice == 2) {
            viewAllProducts(inventory, totalProducts);
        } else if (choice == 3) {
            updateProductQuantity(inventory, totalProducts);
        } else if (choice == 4) {
            searchProductByID(inventory, totalProducts);
        } else if (choice == 5) {
            searchProductByName(inventory, totalProducts);
        } else if (choice == 6) {
            searchProductByPriceRange(inventory, totalProducts);
        } else if (choice == 7) {
            deleteProductByID(&inventory, &totalProducts);
        } else if (choice == 8) {
            free(inventory);
            printf("Memory freed and program exited successfully.\n");
            return 0;
        }
    }
}
