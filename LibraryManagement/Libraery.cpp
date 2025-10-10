#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// ---------------- Book Class ----------------
class Book {
public:
    int id;
    string title;
    string author;
    string subject;
    bool isAvailable;

    Book(int id, string title, string author, string subject)
        : id(id), title(title), author(author), subject(subject), isAvailable(true) {}
};

// ---------------- Member Class ----------------
class Member {
public:
    int id;
    string name;
    string email;

    Member(int id, string name, string email)
        : id(id), name(name), email(email) {}
};

// ---------------- Loan Class ----------------
class Loan {
public:
    int id;
    Book* book;
    Member* member;
    time_t issueDate;
    time_t dueDate;
    time_t returnDate;
    double fine;

    Loan(int id, Book* book, Member* member, time_t issueDate, time_t dueDate)
        : id(id), book(book), member(member), issueDate(issueDate), dueDate(dueDate), returnDate(0), fine(0) {}
    
    void returnBook() {
        returnDate = time(nullptr);
        book->isAvailable = true;
        double daysLate = difftime(returnDate, dueDate) / (60*60*24);
        if(daysLate > 0) fine = daysLate * 1; // $1 per day
    }
};

// ---------------- Library Class ----------------
class Library {
public:
    vector<Book*> books;
    vector<Member*> members;
    vector<Loan*> loans;

    void addBook(Book* book) { books.push_back(book); }
    void addMember(Member* member) { members.push_back(member); }

    Loan* lendBook(Book* book, Member* member) {
        if(!book->isAvailable) {
            cout << "Book not available: " << book->title << endl;
            return nullptr;
        }
        time_t now = time(nullptr);
        time_t due = now + 14*24*60*60; // 2 weeks loan
        Loan* loan = new Loan(loans.size()+1, book, member, now, due);
        book->isAvailable = false;
        loans.push_back(loan);
        cout << "Book lent: " << book->title << " to " << member->name << endl;
        return loan;
    }

    void returnBook(Loan* loan) {
        loan->returnBook();
        cout << "Book returned: " << loan->book->title << endl;
        if(loan->fine > 0)
            cout << "Overdue fine: $" << loan->fine << endl;
    }

    void checkOverdue() {
        time_t now = time(nullptr);
        for(auto l : loans){
            if(l->returnDate == 0 && difftime(now, l->dueDate) > 0) {
                double daysLate = difftime(now, l->dueDate)/(60*60*24);
                cout << "Overdue book: " << l->book->title << ", Days overdue: " << daysLate << endl;
            }
        }
    }
};

// ---------------- Main ----------------
int main() {
    Library lib;

    // Books
    Book* b1 = new Book(1,"C++ Basics","Bjarne","Programming");
    Book* b2 = new Book(2,"Data Structures","Mark","CS");
    lib.addBook(b1);
    lib.addBook(b2);

    // Members
    Member* m1 = new Member(1,"Piyush","piyush@gmail.com");
    lib.addMember(m1);

    // Lending Book
    Loan* l1 = lib.lendBook(b1, m1);

    // Returning Book (simulate after some days)
    lib.returnBook(l1);

    // Check Overdue
    lib.checkOverdue();

    return 0;
}
