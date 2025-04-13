// SRM University AP
// CSC-102 : Data Structures and Algorithms
// Even Sem, 2024-2025
// End Sem Project - Problem Set 12
// Shoping Cart Using Trees
// --------------------------------------------------
// Submitted By:
// Section 'S' Group 12
// S Vaishnav AP24110011309
// Paras Bisht AP24110011288
// Om Swaroop Dash AP24110011295
// Sai Nagendra AP24110011268
// ----------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a product node in the BST
struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
    struct Product *left, *right;
};

// Structure to represent a cart item as a linked list node
struct CartItem {
    int id;
    char name[50];
    float price;
    int quantity;
    struct CartItem *next;
};

// Function to create a new product node
struct Product* createProduct(int id, char name[], float price, int quantity) {
    struct Product* newProduct = (struct Product*)malloc(sizeof(struct Product));
    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->left = newProduct->right = NULL;
    return newProduct;
}

// Insert product into BST
struct Product* insertProduct(struct Product* root, int id, char name[], float price, int quantity) {
    if (root == NULL)
        return createProduct(id, name, price, quantity);
    if (id < root->id)
        root->left = insertProduct(root->left, id, name, price, quantity);
    else if (id > root->id)
        root->right = insertProduct(root->right, id, name, price, quantity);
    return root;
}

// Search product by ID
struct Product* searchProduct(struct Product* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (id < root->id)
        return searchProduct(root->left, id);
    return searchProduct(root->right, id);
}

// Display products using in-order traversal
void displayProducts(struct Product* root) {
    if (root != NULL) {
        displayProducts(root->left);
        printf("ID: %d | Name: %s | Price: %.2f | Stock: %d\n", root->id, root->name, root->price, root->quantity);
        displayProducts(root->right);
    }
}

// Add item to cart
void addToCart(struct CartItem** cart, struct Product* product, int quantity) {
    struct CartItem* temp = *cart;
    while (temp != NULL) {
        if (temp->id == product->id) {
            if (quantity > product->quantity) {
                printf("Not enough stock.\n");
                return;
            }
            temp->quantity += quantity;
            product->quantity -= quantity;
            printf("Updated quantity in cart: %s x%d\n", product->name, temp->quantity);
            return;
        }
        temp = temp->next;
    }
    if (product == NULL) {
        printf("Product not found.\n");
        return;
    }
    if (quantity > product->quantity) {
        printf("Not enough stock.\n");
        return;
    }

    // Deduct from stock
    product->quantity -= quantity;

    // Create new cart item
    struct CartItem* newItem = (struct CartItem*)malloc(sizeof(struct CartItem));
    newItem->id = product->id;
    strcpy(newItem->name, product->name);
    newItem->price = product->price;
    newItem->quantity = quantity;
    newItem->next = *cart;
    *cart = newItem;

    printf("Added to cart: %s x%d\n", product->name, quantity);
}

// Remove item from cart
void removeFromCart(struct CartItem** cart, struct Product* root, int id, int removeQty) {
    struct CartItem *temp = *cart, *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Item not found in cart.\n");
        return;
    }

    if (removeQty >= temp->quantity) {
        // Restore full quantity and remove item
        struct Product* product = searchProduct(root, id);
        if (product != NULL) {
            product->quantity += temp->quantity;
        }
        printf("Only %d qty(s) available in the cart.\n", temp->quantity);
        printf("Removing available quantity...\n");
    } else {
        temp->quantity -= removeQty;
        struct Product* product = searchProduct(root, id);
        if (product != NULL) {
            product->quantity += removeQty;
        }
        printf("Removed %d item(s) from cart.\n", removeQty);
        return;
    }
    if (prev == NULL)
        *cart = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Item removed from cart\n");
}

// Display cart contents
void viewCart(struct CartItem* cart) {
    float total = 0;
    if (cart == NULL) {
        printf("Cart is empty.\n");
        return;
    }
    printf("Your Cart:\n");
    while (cart != NULL) {
        printf("ID: %d | Name: %s | Quantity: %d | Price: %.2f\n", cart->id, cart->name, cart->quantity, cart->price);
        total += cart->price * cart->quantity;
        cart = cart->next;
    }
    printf("Total: %.2f\n", total);
}

// Checkout
void checkout(struct CartItem** cart) {
    float total = 0;
    struct CartItem* temp;

    printf("Order Summary:\n");
    while (*cart != NULL) {
        temp = *cart;
        printf("Product: %s | Quantity: %d | Price: %.2f\n", temp->name, temp->quantity, temp->price);
        total += temp->price * temp->quantity;
        *cart = temp->next;
        free(temp);
    }
    printf("Total Cost: %.2f\n", total);
    if(total == 0){
        printf("Cart is empty. Please add items to cart.\n");
    }
    else{
        printf("Thank you for shopping!\n");
    }
}

// Main function with menu
int main() {
    struct Product* root = NULL;
    struct CartItem* cart = NULL;
    int choice, id, qty, removeQty;
    float price;
    char name[50];

    // Preloaded products
    root = insertProduct(root, 101, "Arduino Uno", 750, 10);
    root = insertProduct(root, 102, "ESP8266", 600, 20);
    root = insertProduct(root, 103, "Raspberry Pi (1 GB Ram)", 2500, 5);
    root = insertProduct(root, 104, "OLED Display (SSD1306)", 200, 15);
    root = insertProduct(root, 105, "Li-ion 18650 Cells (3.7V 1200mAh)", 60, 100);
    root = insertProduct(root, 106, "Breadboard & Jumper Wires", 800, 15);

    while (1) {
        printf("\n--- Online Shopping Cart ---\n");
        printf("1. View Products\n2. Add to Cart\n3. Remove from Cart\n4. View Cart\n5. Checkout\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n--------------\n");
                displayProducts(root);
                printf("--------------\n");
                break;
            case 2:
                printf("Enter Product ID to add to cart: ");
                scanf("%d", &id);
                printf("Enter quantity to add: ");
                scanf("%d", &qty);
                printf("\n--------------\n");
                addToCart(&cart, searchProduct(root, id), qty);
                printf("--------------\n");
                break;
            case 3:
                printf("Enter Product ID to remove from cart: ");
                scanf("%d", &id);
                printf("Enter quantity to remove: ");
                scanf("%d", &removeQty);
                printf("\n--------------\n");
                removeFromCart(&cart, root, id, removeQty);
                printf("--------------\n");
                break;
            case 4:
                printf("\n--------------\n");
                viewCart(cart);
                printf("--------------\n");
                break;
            case 5:
                printf("\n--------------\n");
                checkout(&cart);
                printf("--------------\n");
                break;
            case 6:
                printf("\n--------------\n");
                printf("Exiting...\n");
                printf("--------------\n");
                printf("\n");
                exit(0);
                break;
            default:
                printf("\n--------------\n");
                printf("Select a valid operation.\n");
                printf("--------------\n");
                break;
        }
    }
    return 0;
}
