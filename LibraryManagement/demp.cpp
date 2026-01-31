#include <bits/stdc++.h>
using namespace std;

class FinePolicy {
public:
    virtual double calculateFine(time_t due, time_t returned) = 0;
    virtual ~FinePolicy() = default;
};

class DefaultFinePolicy : public FinePolicy {
public:
    double calculateFine(time_t due, time_t returned) override {
        double daysLate = difftime(returned, due) / (60*60*24);
        return daysLate > 0 ? daysLate*1.0 : 0.0;
    }
};

class Book {
public:
    int id;
    string title, author, subject;
    bool available = true;

    Book(int i, string t, string a, string s) {
        id = i;
        title = t;
        author = a;
        subject = s;
    }
};

class Member {
public:
    int id;
    string name, email;

    Member(int i, string n, string e) {
        id = i;
        name = n;
        email = e;
    }
};

class Loan {
public:
    int id;
    Book* book;
    Member* member;
    time_t issue, due, returnDate = 0;
    double fines = 0.0;

    Loan(int i, Book* b, Member* m, time_t is, time_t du) {
        id = i;
        book = b;
        member = m;
        issue = is;
        due = du;
    }
};

class Library {
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;
    FinePolicy* fine;

public:
    Library() {
        fine = new DefaultFinePolicy();
    }
    ~Library() {
        delete fine;
    }

    void addBook(Book b) {
        books.push_back(b);
    }

    void addMember(Member m) {
        members.push_back(m);
    }

    Loan* lendBook(int bookId, int memberId) {
        auto bookIt = find_if(books.begin(), books.end(), [&](Book &b) { return b.id == bookId; });
        auto memberIt = find_if(members.begin(), members.end(), [&](Member &m) { return m.id == memberId; });

        if(bookIt == books.end() || memberIt == members.end()) {
            cout << "Invalid book or member ID" << endl;
            return nullptr;
        }

        if(!bookIt->available) {
            cout << "Book not available: " << bookIt->title << endl;
            return nullptr;
        }

        time_t now = time(NULL);
        time_t due = now + 14*24*60*60; // 2 weeks

        loans.emplace_back(loans.size() + 1, &(*bookIt), &(*memberIt), now, due);
        bookIt->available = false;

        cout << "✅ Book lent: " << bookIt->title << " → " << memberIt->name << "\n";
        return &loans.back();
    }

    void returnBook(int loanId) {
        if(loanId <= 0 || loanId > (int)loans.size()) {
            cout << "Invalid loan ID.\n";
            return;
        }

        Loan &loan = loans[loanId - 1];
        if(loan.returnDate != 0) {
            cout << "Book already returned.\n";
            return;
        }

        loan.returnDate = time(NULL);
        loan.fines = fine->calculateFine(loan.due, loan.returnDate);
        loan.book->available = true;

        cout << "Book returned: " << loan.book->title << "\n";
        if(loan.fines > 0) {
            cout << "Overdue fine: $" << loan.fines << endl;
        }
    }

    void checkOverdue() {
        time_t now = time(NULL);
        for(Loan &l : loans) {
            if(l.returnDate == 0 && difftime(now, l.due) > 0) {
                double daysLate = difftime(now, l.due) / (60*60*24);
                cout << "Overdue: " << l.book->title << " (" << daysLate << " days late)" << endl;
            }
        }
    }

    void listAvailableBooks() {
        cout << "Available Books:" << endl;
        for(Book &b : books) {
            if(b.available) {
                cout << "- " << b.title << " by " << b.author << endl;
            }
        }
    }
};

int main() {
    Library lib;

    lib.addBook(Book(1, "C++ Basics", "Bjarne", "Programming"));
    lib.addBook(Book(2, "Data Structures", "Mark", "CS"));
    lib.addMember(Member(1, "Piyush", "piyush@gmail.com"));

    lib.listAvailableBooks();
    Loan* loan = lib.lendBook(1, 1);
    if(loan) lib.returnBook(loan->id);
    lib.checkOverdue();
    lib.listAvailableBooks();

    return 0;
}
