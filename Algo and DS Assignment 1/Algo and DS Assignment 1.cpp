#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

struct Product {
    std::string id;
    std::string name;
    double price{};
    std::string category;
};

const int maxProducts = 100; // Maximum number of products

// Global variables
Product products[maxProducts];
int numProducts = 0;

void load(const std::string& filename);
void addProduct(const Product& newProduct);
void updateProduct(const std::string& productId, const Product& updatedProduct);
void removeProduct(const std::string& productId);
int searchProductById(const std::string& productId);
int searchProductByName(const std::string& productName);
void insertionSortByPrice();

int partition(int low, int high);
void swapProducts(Product& a, Product& b);
void printProducts();

int main() {
    load("product_data_best.txt");
    int action;

    std::string newID;
    std::string newName;
    double newPrice;
    std::string newCategory;

    std::string searchID;
    std::string searchName;

    choice:
    std::cout << "What would you like to do? Enter 1 to add a product, 2 to update an existing product, 3 to delete a product, 4 to search for a product, 5 to insertion sort the database, or 6 to exit." << std::endl;
    std::cin >> action;
    
    if (action < 1 || action > 6) {
        std::cout << "Invalid input." << std::endl;
        goto choice;
    }

    switch (action) {
    case 1:
        std::cout << "Please enter the new ID number, name, price, and category of the item you are adding, in that order." << std::endl;
        std::cin >> newID;
        std::cin >> newName;
        std::cin >> newPrice;
        std::cin >> newCategory;
        
        addProduct({newID, newName, newPrice, newCategory});
        //std::cout << "New item: " << products[searchProductById(searchID)].id << ", " << products[searchProductById(searchID)].name << ", " << products[searchProductById(searchID)].price << ", " << products[searchProductById(searchID)].category << std::endl;

        break;
    case 2:
        std::cout << "Please enter the current ID number of the product you'd like to update." << std::endl;
        std::cin >> searchID;

        std::cout << "Please enter the new ID, name, price, and category of that item." << std::endl;
        std::cin >> newID;
        std::cin >> newName;
        std::cin >> newPrice;
        std::cin >> newCategory;

        updateProduct(searchID, {newID, newName, newPrice, newCategory});
        //std::cout << "Item: " << products[searchProductById(searchID)].id << ", " << products[searchProductById(searchID)].name << ", " << products[searchProductById(searchID)].price << ", " << products[searchProductById(searchID)].category << std::endl;

        break;
    case 3:
        std::cout << "Please enter the ID of the product you'd like to remove." << std::endl;
        std::cin >> searchID;

        removeProduct(searchID);

        break;
    case 4:
        int choiceTwo;

        repeat:
        std::cout << "Would you like to search via ID or name? Enter 1 for ID or 2 for name." << std::endl;
        std::cin >> choiceTwo;

        if (choiceTwo < 1 || choiceTwo > 2) {
            std::cout << "Invalid input." << std::endl;
            goto repeat;
        }

        if (choiceTwo == 1) {
            std::cout << "Please enter the ID of the product you would like to search for." << std::endl;
            std::cin >> searchID;

            std::cout << "Item: " << products[searchProductById(searchID)].id << ", " << products[searchProductById(searchID)].name << ", " << products[searchProductById(searchID)].price << ", " << products[searchProductById(searchID)].category << std::endl;
        }
        else {
            std::cout << "Please enter the name of the product you would like to search for." << std::endl;
            std::cin >> searchName;

            std::cout << "Item: " << products[searchProductByName(searchName)].id << ", " << products[searchProductByName(searchName)].name << ", " << products[searchProductByName(searchName)].price << ", " << products[searchProductByName(searchName)].category << std::endl;
        }

        break;
    case 5:
        insertionSortByPrice();

        break;
    case 6:
        return 0;
    }

    goto choice;
}

void load(const std::string& filename) {
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string id, name, category;
        double price;

        if (std::getline(iss, id, ',') && std::getline(iss, name, ',') && (iss >> price) && std::getline(iss >> std::ws, category)) {

            if (numProducts < maxProducts) {
                products[numProducts] = {id, name, price, category};
                numProducts++;
            }
            else {
                std::cerr << "Maximum number of products reached." << std::endl;
                break;
            }
        }
        else {
            std::cerr << "Error getting data from line: " << line << std::endl;
        }
    }

    printProducts();

    infile.close();
}

void addProduct(const Product& newProduct) {
    if (numProducts < maxProducts) {
        products[numProducts++] = newProduct;
        printProducts();
    }
    else {
        std::cerr << "Maximum number of products (100) reached." << std::endl;
    }
}

void updateProduct(const std::string& productId, const Product& updatedProduct) {
    int index = searchProductById(productId);

    if (index != -1) {
        products[index] = updatedProduct;
        printProducts();
    }
    else {
        std::cerr << "Product not found." << std::endl;
    }
}

void removeProduct(const std::string& productId) {
    int index = searchProductById(productId);

    if (index != -1) {
        for (int i = index; i < numProducts - 1; ++i) {
            products[i] = products[i + 1];
        }
        numProducts--;
        printProducts();
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

void insertionSortByPrice() {
    auto start = std::chrono::steady_clock::now();

    for (int i = 1; i < numProducts; ++i) {
        Product key = products[i];
        int j = i - 1;

        while (j >= 0 && products[j].price > key.price) {
            products[j + 1] = products[j];
            j = j - 1;
        }

        products[j + 1] = key;
    }

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    printProducts();

    std::cout << "It took " << std::fixed << std::setprecision(2) << duration.count() << " microseconds." << std::endl;
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
