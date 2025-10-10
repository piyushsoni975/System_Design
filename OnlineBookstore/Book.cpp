#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// ---------------- Book Class ----------------
class Book {
private:
    int id;
    string title;
    string author;
    string subject;
    double price;
    int stock;
public:
    Book(int id, string title, string author, string subject, double price, int stock)
        : id(id), title(title), author(author), subject(subject), price(price), stock(stock) {}

    int getId() { return id; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getSubject() { return subject; }
    double getPrice() { return price; }
    int getStock() { return stock; }

    void updateStock(int quantity) {
        stock += quantity;
        if(stock < 5) {
            cout << "Restock required for book: " << title << endl;
        }
    }
};

// ---------------- Patron Class ----------------
class Patron {
private:
    int id;
    string name;
    string email;
public:
    Patron(int id, string name, string email) : id(id), name(name), email(email) {}
    int getId() { return id; }
    string getName() { return name; }
    string getEmail() { return email; }
};

// ---------------- Order Class ----------------
class Order {
private:
    int id;
    Patron* patron;
    vector<Book*> books;
    double totalAmount;
    bool isPaid;
    bool isFulfilled;
public:
    Order(int id, Patron* patron) : id(id), patron(patron), totalAmount(0), isPaid(false), isFulfilled(false) {}

    void addBook(Book* book) {
        if(book->getStock() > 0){
            books.push_back(book);
        } else {
            cout << "Not enough stock for " << book->getTitle() << endl;
        }
    }

    double calculateTotal() {
        totalAmount = 0;
        for(auto b : books) totalAmount += b->getPrice();
        return totalAmount;
    }

    void processPayment() {
        totalAmount = calculateTotal();
        isPaid = true;
        cout << "Payment of $" << totalAmount << " processed for order " << id << endl;
    }

    void fulfillOrder() {
        if(isPaid){
            for(auto b : books){
                b->updateStock(-1); // assuming quantity = 1
            }
            isFulfilled = true;
            cout << "Order " << id << " fulfilled." << endl;
        } else {
            cout << "Payment not done. Cannot fulfill order." << endl;
        }
    }
};

// ---------------- BookStore Class ----------------
class BookStore {
private:
    vector<Book*> books;
    unordered_map<string, vector<Book*>> titleMap;
    unordered_map<string, vector<Book*>> authorMap;
    unordered_map<string, vector<Book*>> subjectMap;
public:
    void addBook(Book* book) {
        books.push_back(book);
        titleMap[book->getTitle()].push_back(book);
        authorMap[book->getAuthor()].push_back(book);
        subjectMap[book->getSubject()].push_back(book);
    }

    vector<Book*> searchByTitle(const string& title) {
        return titleMap.count(title) ? titleMap[title] : vector<Book*>{};
    }
    vector<Book*> searchByAuthor(const string& author) {
        return authorMap.count(author) ? authorMap[author] : vector<Book*>{};
    }
    vector<Book*> searchBySubject(const string& subject) {
        return subjectMap.count(subject) ? subjectMap[subject] : vector<Book*>{};
    }

    void checkInventory() {
        for(auto b : books){
            if(b->getStock() < 5) cout << "Restock needed for: " << b->getTitle() << endl;
        }
    }
};

// ---------------- Main Function ----------------
int main() {
    BookStore store;

    // Add Books
    Book* b1 = new Book(1, "C++ Basics", "Bjarne", "Programming", 50, 10);
    Book* b2 = new Book(2, "Data Structures", "Mark", "CS", 40, 2);
    Book* b3 = new Book(3, "Algorithms", "CLRS", "CS", 60, 6);

    store.addBook(b1);
    store.addBook(b2);
    store.addBook(b3);

    // Create Patron
    Patron* p1 = new Patron(1, "Piyush", "piyush@gmail.com");

    // Place Order
    Order* o1 = new Order(1, p1);
    o1->addBook(b1);
    o1->addBook(b2);
    o1->processPayment();
    o1->fulfillOrder();

    // Search Books
    auto results = store.searchByTitle("C++ Basics");
    cout << "Search Results for 'C++ Basics': " << results.size() << " book(s) found." << endl;

    // Check Inventory
    store.checkInventory();

    return 0;
}
