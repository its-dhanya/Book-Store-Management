//Please use C++ Version 14 or Higher

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// Abstract base class representing a general inventory item.
// Implements the interface for all items in the inventory.

class InventoryItem{
public:
    virtual ~InventoryItem() = default;   
    virtual void displayDetails() const = 0; 
    virtual const std::string& getISBN() const = 0;
    virtual void updateStock(int quantity) = 0;
};

//Class representing a book in the inventory.

class Book: public InventoryItem{
protected:
    std::string title;
    std::string author;
    std::string ISBN;
    double price;
    double cost;
    int stock;
    
public:
    Book(const std::string &t , const std::string &a , const std::string& i, double p, double c , int s):
        title(t), author(a), ISBN(i), price(p), cost(c), stock(s) {}

    virtual ~Book() = default;

    void displayDetails() const override{
        std::cout << "Title: " << title << "\nAuthor: " << author
                  << "\nISBN: "<< ISBN << "\nPrice: " << price
                  << "\nStock:"<< stock <<"\n"<< std::endl;
    }
    const std:: string& getISBN() const override {return ISBN;}
    
    void updateStock(int quantity) override{
        if (stock + quantity < 0) {
            throw std::runtime_error("Insufficient Stock!");
        }
        stock += quantity;
    }
};

// Class representing a magazine, derived from the `Book` class

class Magazine: public Book{
private:
    std::string issue;
    std::string publisher;

public:
    Magazine(const std::string &t , const std::string &a ,const std::string& i, double p, double c , int s,
             const std::string &is , const std::string &pub):
             Book(t , a , i , p , c , s) , issue(is) , publisher(pub) {}
    
    void displayDetails() const override {
        Book::displayDetails();
        std::cout<< "Issue: "<< issue << "\nPublisher: " << publisher << "\n" << std::endl;  
    }
};

// Abstract class representing the interface for an inventory system.

class IntInventory {
public:
    virtual ~IntInventory() = default;
    virtual void addItem(const std:: shared_ptr<InventoryItem>& item) = 0;
    virtual void displayInventory() const = 0;
    virtual std::shared_ptr<InventoryItem> findItemByISBN(const std::string &isbn) = 0;
};

// Class implementing the inventory system.
class Inventory: public IntInventory{
private:
    std::vector <std::shared_ptr <InventoryItem> > items;

public:
    void addItem(const std::shared_ptr<InventoryItem> &item) override{
        items.push_back(item);
    }

    void displayInventory() const override{
        if (items.empty()){
            std::cout << "Inventory is empty." <<std::endl;
        }
        else{
            for(const auto &item : items){
                item->displayDetails();
            }
        }
    }

    std::shared_ptr<InventoryItem> findItemByISBN(const std::string& isbn) override{
        for(auto &item: items){
            if (item->getISBN() == isbn){
                return item;
            }
        }
        return nullptr;
    }
};

// Abstract base class for all people interacting with the bookstore.

class Person{
protected:
    std::string name;
public:
    Person(const std::string &n): name(n) {}
    virtual ~Person() = default;
    virtual void displayname() const = 0;
};

// Store manager responsible for managing inventory operations.

class StoreManager : public Person{
private:
    std::unique_ptr<IntInventory> &inventory;

public:
    StoreManager(const std::string &n, std::unique_ptr<IntInventory> &inv):
        Person(n) , inventory(inv) {}
    
    void addItem(const std::shared_ptr<InventoryItem> &item) {
        inventory->addItem(item);
        std::cout << name << "Added an item to the inventory\n";
    }

    void displayname() const override{
        std::cout << name << " manages the inventory and oversees operations.\n";

    }

};

//Class of Cashier is responsible for maintaining current stock quantity.

class Cashier: public Person {
public:
    Cashier(const std::string &n): Person(n) {}

    void sellItem(std::unique_ptr<IntInventory> &inventory){
        std::string isbn;
        int quantity;
        
        std::cin.ignore();
        std::cout << "Enter ISBN of the item: ";
        std::getline(std::cin, isbn);
        std::cout << "Enter quantity : ";
        std::cin >> quantity;

        auto item = inventory ->findItemByISBN(isbn);
        if (item){
            try{
                item->updateStock(-quantity);
                std::cout << name << "Sold!" << quantity << "copies of" << isbn << "successfully.\n";
            } catch(const std::exception &e){
                std::cerr << e.what() << std::endl;
            }
        } else{
            std::cout<< "Item with ISBN " << isbn << " not found in inventory.\n";
        }
    }

    void displayname() const override{
        std::cout << name << " handles sales and customer interactions.\n";
    }
};

//Customer Class to browse the Inventory.

class Customer : public Person {
public:
    Customer(const std::string &n): Person(n) {}

    void browseInventory(const IntInventory &inventory) const {
        std::cout << name << " is browsing the inventory.\n";
        inventory.displayInventory();
    }

    void displayname() const override {
        std::cout << name << " browses and purchases items.\n";
    }
};

// Represents the overall bookstore system, managing the inventory, manager, and cashier.

class Bookstore {
private:
    std::unique_ptr<IntInventory> inventory;
    StoreManager manager;
    Cashier cashier;

public:
    Bookstore()
        : inventory(std::make_unique<Inventory>()),
          manager("Alice (Manager)", inventory),
          cashier("Bob (Cashier)") {}

    void run() {
        while (true) {
            std::cout << "\nWelcome to the Bookstore System\n";
            std::cout << "1. Manager\n2. Customer\n3. Exit\nChoose your role: ";
            int roleChoice;
            std::cin >> roleChoice;

            switch (roleChoice) {
                case 1:  
                    managerMenu();
                    break;

                case 2:
                    customerMenu();
                    break;

                case 3:  
                    std::cout <<"Goodbye!\n";
                    return;

                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void managerMenu() {
        while (true) {
            std::cout << "\nManager Menu:\n1. Add Item\n2. View Inventory\n3. Go Back\nChoose an option: ";
            int managerChoice;
            std::cin >> managerChoice;

            switch (managerChoice) {
                case 1:
                    addItem();
                    break;

                case 2:
                    inventory->displayInventory();
                    break;

                case 3:
                    return;

                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void addItem() {
        std::cout << "\nAdding an item...\n";
        std::string title, author, isbn, issue, publisher;
        double price, cost;
        int stock;
        int itemType;

        std::cout << "Enter item type (1 for Book, 2 for Magazine): ";
        std::cin >> itemType;

        std::cin.ignore();  
        
        std::cout << "Enter Title: ";
        std::getline(std::cin, title);
        
        std::cout << "Enter Author: ";
        std::getline(std::cin, author);
        std::cout << "Enter ISBN: ";
        std::getline(std::cin, isbn);
        std::cout << "Enter Price: ";
        std::cin >> price;
        std::cout << "Enter Cost: ";
        std::cin >> cost;
        std::cout << "Enter Stock: ";
        std::cin >> stock;

        if (itemType == 1) {
            auto newBook = std::make_shared<Book>(title, author, isbn, price, cost, stock);
            inventory->addItem(newBook);
            std::cout << "Book added to inventory.\n";
        } else if (itemType == 2) {
            std::cin.ignore();
            std::cout << "Enter Issue: ";
            std::getline(std::cin, issue);
            std::cout << "Enter Publisher: ";
            std::getline(std::cin, publisher);

            auto newMagazine = std::make_shared<Magazine>(title, author, isbn, price, cost, stock, issue, publisher);
            inventory->addItem(newMagazine);
            std::cout << "Magazine added to inventory.\n";
        } else {
            std::cout << "Invalid item type. Please try again.\n";
        }
    }

    void customerMenu() {
        while (true) {
            std::cout << "\nCustomer Menu:\n1. Buy Item\n2. Browse Inventory\n3. Go Back\nChoose an option: ";
            int customerChoice;
            std::cin >> customerChoice;

            switch (customerChoice) {
                case 1:
                    cashier.sellItem(inventory);
                    break;

                case 2: {
                    std::string customerName;
                    std::cin.ignore();  // Clear input buffer
                    std::cout << "Enter your name: ";
                    std::getline(std::cin, customerName);

                    Customer customer(customerName);
                    customer.browseInventory(*inventory);
                    break;
                }

                case 3:
                    return;

                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    Bookstore store;
    store.run();
    return 0;
}
//Tried to apply concepts that were taught in class as much as I could.
// 1. OOPs Concepts (Abstraction , Inheritance, Polymorphism , Encapsulation)
// 2. Smart Pointers for better memory management.
// 3. Exception Handling
// 4. Virtual Functions
// 5. SOLID Principles 

