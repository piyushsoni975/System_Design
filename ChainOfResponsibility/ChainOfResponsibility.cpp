#include <bits/stdc++.h>
using namespace std;

// Abstract Handler
class Handler {
protected:
    Handler* next; // next handler in the chain

public:
    Handler() : next(nullptr) {}
    virtual ~Handler() {}

    void setNext(Handler* nxt) {
        next = nxt;
    }

    // Return true if handled, else false
    virtual bool handleRequest(int request) = 0;
};

// Concrete Handler 1
class ConcreteHandler1 : public Handler {
public:
    bool handleRequest(int request) override {
        if (request < 10) {
            cout << "ConcreteHandler1 handled request " << request << endl;
            return true;
        } else if (next != nullptr) {
            return next->handleRequest(request);
        }
        return false;
    }
};

// Concrete Handler 2
class ConcreteHandler2 : public Handler {
public:
    bool handleRequest(int request) override {
        if (request >= 10 && request < 20) {
            cout << "ConcreteHandler2 handled request " << request << endl;
            return true;
        } else if (next != nullptr) {
            return next->handleRequest(request);
        }
        return false;
    }
};

// Concrete Handler 3
class ConcreteHandler3 : public Handler {
public:
    bool handleRequest(int request) override {
        if (request >= 20) {
            cout << "ConcreteHandler3 handled request " << request << endl;
            return true;
        } else if (next != nullptr) {
            return next->handleRequest(request);
        }
        return false;
    }
};

// Client
int main() {
    Handler* h1 = new ConcreteHandler1();
    Handler* h2 = new ConcreteHandler2();
    Handler* h3 = new ConcreteHandler3();

    // Build chain: h1 -> h2 -> h3
    h1->setNext(h2);
    h2->setNext(h3);

    vector<int> requests = {2, 5, 14, 22, 100, -5};

    for (int req : requests) {
        cout << "Request " << req << ": ";
        bool handled = h1->handleRequest(req);
        if (!handled) {
            cout << "❌ No handler could process this request." << endl;
        }
    }

    delete h1;
    delete h2;
    delete h3;

    return 0;
}
