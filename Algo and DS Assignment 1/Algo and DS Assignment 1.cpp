#include <iostream>
#include <fstream>
#include <string>

struct Product {
    std::string id;
    std::string name;
    double price{};
    std::string category;
};

const int MAX_PRODUCTS = 100; // Maximum number of products

// Global variables to hold product data
Product products[MAX_PRODUCTS];
int numProducts = 0;

// Function prototypes
void loadDataFromFile(const std::string& filename);
void insertProduct(const Product& newProduct);
void updateProduct(const std::string& productId, const Product& updatedProduct);
void deleteProduct(const std::string& productId);
int searchProductById(const std::string& productId);
int searchProductByName(const std::string& productName);
void bubbleSortByPrice();
void insertionSortByPrice();
void quickSortByPrice(int low, int high);
int partition(int low, int high); // Forward declaration
void swapProducts(Product& a, Product& b);
void printProducts();

int main() {
    loadDataFromFile("product_data.txt");

    // insertProduct({"101", "New Product", 10.99, "Miscellaneous"});
    // updateProduct("101", {"101", "Updated Product", 15.99, "Miscellaneous"});
    // deleteProduct("101");
    // int foundIndex = searchProductById("101");
    // bubbleSortByPrice();
    // insertionSortByPrice();
    // quickSortByPrice(0, numProducts - 1);
    // printProducts();

    return 0;
}

void loadDataFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
    }

    while (infile >> products[numProducts].id >> products[numProducts].name >> products[numProducts].price >> products[numProducts].category) {
        numProducts++;
    }

    infile.close();
}

void insertProduct(const Product& newProduct) {
    if (numProducts < MAX_PRODUCTS) {
        products[numProducts++] = newProduct;
    }
    else {
        std::cerr << "Maximum number of products reached." << std::endl;
    }
}

void updateProduct(const std::string& productId, const Product& updatedProduct) {
    int index = searchProductById(productId);
    if (index != -1) {
        products[index] = updatedProduct;
    }
    else {
        std::cerr << "Product not found." << std::endl;
    }
}

void deleteProduct(const std::string& productId) {
    int index = searchProductById(productId);
    if (index != -1) {
        for (int i = index; i < numProducts - 1; ++i) {
            products[i] = products[i + 1];
        }
        numProducts--;
    }
    else {
        std::cerr << "Product not found." << std::endl;
    }
}

int searchProductById(const std::string& productId) {
    for (int i = 0; i < numProducts; ++i) {
        if (products[i].id == productId) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

int searchProductByName(const std::string& productName) {
    for (int i = 0; i < numProducts; ++i) {
        if (products[i].name == productName) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

void bubbleSortByPrice() {
    for (int i = 0; i < numProducts - 1; ++i) {
        for (int j = 0; j < numProducts - i - 1; ++j) {
            if (products[j].price > products[j + 1].price) {
                swapProducts(products[j], products[j + 1]);
            }
        }
    }
}

void insertionSortByPrice() {
    for (int i = 1; i < numProducts; ++i) {
        Product key = products[i];
        int j = i - 1;
        while (j >= 0 && products[j].price > key.price) {
            products[j + 1] = products[j];
            j = j - 1;
        }
        products[j + 1] = key;
    }
}

void quickSortByPrice(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quickSortByPrice(low, pi - 1);
        quickSortByPrice(pi + 1, high);
    }
}

int partition(int low, int high) {
    double pivot = products[high].price;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (products[j].price < pivot) {
            ++i;
            swapProducts(products[i], products[j]);
        }
    }
    swapProducts(products[i + 1], products[high]);
    return i + 1;
}

void swapProducts(Product& a, Product& b) {
    Product temp = a;
    a = b;
    b = temp;
}

void printProducts() {
    for (int i = 0; i < numProducts; ++i) {
        std::cout << products[i].id << ", " << products[i].name << ", " << products[i].price << ", " << products[i].category << std::endl;
    }
}
