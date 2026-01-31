#include <bits/stdc++.h>
using namespace std;

// ---------------- Fine Policy (Strategy Pattern) ----------------
class FinePolicy {
public:
    virtual double calculateFine(time_t due, time_t returned) = 0;
    virtual ~FinePolicy() = default;
};

class DefaultFinePolicy : public FinePolicy {
public:
    double calculateFine(time_t due, time_t returned) override {
        double daysLate = difftime(returned, due) / (60 * 60 * 24);
        return daysLate > 0 ? daysLate * 1.0 : 0.0; // $1 per day
    }
};

// ---------------- Book Class ----------------
class Book {
public:
    int id;
    string title, author, subject;
    bool available = true;

    Book(int id, const string& t, const string& a, const string& s)
        : id(id), title(t), author(a), subject(s) {}
};

// ---------------- Member Class ----------------
class Member {
public:
    int id;
    string name, email;

    Member(int id, const string& n, const string& e)
        : id(id), name(n), email(e) {}
};

// ---------------- Loan Class ----------------
class Loan {
public:
    int id;
    Book* book;
    Member* member;
    time_t issueDate, dueDate, returnDate = 0;
    double fine = 0.0;

    Loan(int id, Book* b, Member* m, time_t issue, time_t due)
        : id(id), book(b), member(m), issueDate(issue), dueDate(due) {}
};

// ---------------- Library Class ----------------
class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;
    FinePolicy* finePolicy; // raw pointer

public:
    Library() {
        finePolicy = new DefaultFinePolicy();
    }

    ~Library() {
        delete finePolicy; // clean up
    }

    void addBook(const Book& b) { books.push_back(b); }
    void addMember(const Member& m) { members.push_back(m); }

    Loan* lendBook(int bookId, int memberId) {
        auto bookIt = find_if(books.begin(), books.end(), [&](Book& b){ return b.id == bookId; });
        auto memberIt = find_if(members.begin(), members.end(), [&](Member& m){ return m.id == memberId; });

        if (bookIt == books.end() || memberIt == members.end()) {
            cout << "Invalid book or member ID.\n";
            return nullptr;
        }
        if (!bookIt->available) {
            cout << "Book not available: " << bookIt->title << "\n";
            return nullptr;
        }

        time_t now = time(nullptr); 
        time_t due = now + 14 * 24 * 60 * 60; // 2 weeks
        loans.emplace_back(loans.size() + 1, &(*bookIt), &(*memberIt), now, due);
        bookIt->available = false;

        cout << "✅ Book lent: " << bookIt->title << " → " << memberIt->name << "\n";
        return &loans.back();
    }

    void returnBook(int loanId) {
        if (loanId <= 0 || loanId > (int)loans.size()) {
            cout << "Invalid loan ID.\n";
            return;
        }
        Loan& loan = loans[loanId - 1];
        if (loan.returnDate != 0) {
            cout << "Book already returned.\n";
            return;
        }

        loan.returnDate = time(nullptr);
        loan.fine = finePolicy->calculateFine(loan.dueDate, loan.returnDate);
        loan.book->available = true;

        cout << "📚 Book returned: " << loan.book->title << "\n";
        if (loan.fine > 0)
            cout << "⚠️ Overdue fine: $" << loan.fine << "\n";
    }

    void checkOverdue() {
        time_t now = time(nullptr);
        for (Loan& l : loans) {
            if (l.returnDate == 0 && difftime(now, l.dueDate) > 0) {
                double daysLate = difftime(now, l.dueDate) / (60 * 60 * 24);
                cout << "⏰ Overdue: " << l.book->title << " (" << daysLate << " days late)\n";
            }
        }
    }

    void listAvailableBooks() {
        cout << "\n📖 Available Books:\n";
        for (Book& b : books)
            if (b.available)
                cout << "- " << b.title << " by " << b.author << "\n";
    }
};

// ---------------- Main ----------------
int main() {
    Library lib;

    lib.addBook(Book(1, "C++ Basics", "Bjarne", "Programming"));
    lib.addBook(Book(2, "Data Structures", "Mark", "CS"));
    lib.addMember(Member(1, "Piyush", "piyush@gmail.com"));

    lib.listAvailableBooks();
    Loan* loan = lib.lendBook(1, 1);
    if (loan) lib.returnBook(loan->id);
    lib.checkOverdue();
    lib.listAvailableBooks();

    return 0;
}
