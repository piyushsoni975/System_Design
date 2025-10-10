#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ---------------- Seat basics ----------------
enum SeatState { FREE, HELD, SOLD };

const char* toStr(SeatState s) {
    if (s == FREE) return "FREE";
    if (s == HELD) return "HELD";
    return "SOLD";
}

// Ek show ke seats ka bahut simple store (SRP: sirf seat state sambhale)
class ShowInventory {
public:
    ShowInventory(int totalSeats = 20) {
        seats.assign(totalSeats + 1, FREE); // 1-based indexing
    }

    // sare seats FREE hon to ek saath HOLD karo
    bool holdSeats(const vector<int>& ids) {
        // check
        for (int s : ids) {
            if (s <= 0 || s >= (int)seats.size()) return false;
            if (seats[s] != FREE) return false;
        }
        // mark
        for (int s : ids) seats[s] = HELD;
        return true;
    }

    // held -> sold (confirm)
    bool confirmSeats(const vector<int>& ids) {
        for (int s : ids) {
            if (s <= 0 || s >= (int)seats.size()) return false;
            if (seats[s] != HELD) return false;
        }
        for (int s : ids) seats[s] = SOLD;
        return true;
    }

    // held -> free (cancel)
    void releaseSeats(const vector<int>& ids) {
        for (int s : ids) if (s > 0 && s < (int)seats.size() && seats[s] == HELD) seats[s] = FREE;
    }

    void printFirstN(int n = 10) {
        cout << "Seats (first " << n << "): ";
        for (int i = 1; i <= n && i < (int)seats.size(); ++i) {
            cout << i << ":" << toStr(seats[i]) << "  ";
        }
        cout << "\n";
    }

private:
    vector<SeatState> seats;
};

// --------------- Strategy: Pricing (OCP) ---------------
class IPricingStrategy {
public:
    virtual ~IPricingStrategy() {}
    virtual double compute(double basePerSeat, int count) = 0;
    virtual const char* name() = 0;
};

class FlatPricing : public IPricingStrategy {
public:
    double compute(double base, int count) override { return base * count; }
    const char* name() override { return "FlatPricing"; }
};

class SurgePricing : public IPricingStrategy {
public:
    SurgePricing(double m) : mul(m) {}
    double compute(double base, int count) override { return base * mul * count; }
    const char* name() override { return "SurgePricing"; }
private:
    double mul;
};

// --------------- Strategy: Payment (OCP) ---------------
enum PaymentStatus { P_SUCCESS, P_FAIL };

class IPaymentStrategy {
public:
    virtual ~IPaymentStrategy() {}
    virtual PaymentStatus pay(double amount, const string& userId) = 0;
    virtual const char* name() = 0;
};

class CardPayment : public IPaymentStrategy {
public:
    PaymentStatus pay(double amount, const string& userId) override {
        cout << "[Card] charge " << amount << " for " << userId << "\n";
        return P_SUCCESS;
    }
    const char* name() override { return "CardPayment"; }
};

class WalletPayment : public IPaymentStrategy {
public:
    PaymentStatus pay(double amount, const string& userId) override {
        cout << "[Wallet] deduct " << amount << " for " << userId << "\n";
        return P_SUCCESS;
    }
    const char* name() override { return "WalletPayment"; }
};

// --------------- BookingService (DIP + SRP) ---------------
class BookingService {
public:
    BookingService(ShowInventory& inv, IPricingStrategy* p, IPaymentStrategy* pay, double basePrice)
        : inventory(inv), pricing(p), payment(pay), base(basePrice) {}

    bool hold(const vector<int>& seats) {
        return inventory.holdSeats(seats);
    }

    bool confirm(const string& userId, const vector<int>& seats) {
        // price decide strategy se
        double amount = pricing->compute(base, (int)seats.size());
        cout << "[Pricing:" << pricing->name() << "] total = " << amount << "\n";

        // pay strategy se
        if (payment->pay(amount, userId) != P_SUCCESS) return false;

        // seats ko SOLD karo
        return inventory.confirmSeats(seats);
    }

    // runtime me strategy swap (Open/Closed + DIP)
    void setPricing(IPricingStrategy* p)  { pricing = p; }
    void setPayment(IPaymentStrategy* p)  { payment = p; }

private:
    ShowInventory& inventory;       // SRP: inventory sirf seats sambhalta
    IPricingStrategy* pricing;      // DIP: abstraction par depend
    IPaymentStrategy* payment;      // DIP: abstraction par depend
    double base;
};

// --------------- Demo ---------------
int main() {
    ShowInventory inv(15);      // 15 seats
    inv.printFirstN(8);

    FlatPricing flat;
    CardPayment card;
    BookingService svc(inv, &flat, &card, 150.0);

    // Hold 5,6
    vector<int> s1 = {5, 6};
    cout << "\nHold(5,6): " << (svc.hold(s1) ? "true" : "false") << "\n";
    inv.printFirstN(8);

    // Confirm with Flat + Card
    cout << "\n-- Confirm with Flat + Card --\n";
    bool ok1 = svc.confirm("user_123", s1);
    cout << "Booking: " << (ok1 ? "SUCCESS" : "FAILED") << "\n";
    inv.printFirstN(8);

    // Try again: should fail (already SOLD)
    cout << "\nHold again (5,6): " << (svc.hold(s1) ? "true" : "false") << "\n";

    // Switch to Surge + Wallet
    SurgePricing surge(1.5);
    WalletPayment wallet;
    svc.setPricing(&surge);
    svc.setPayment(&wallet);

    vector<int> s2 = {7, 8};
    cout << "\nHold(7,8): " << (svc.hold(s2) ? "true" : "false") << "\n";
    cout << "-- Confirm with Surge + Wallet --\n";
    bool ok2 = svc.confirm("user_999", s2);
    cout << "Booking2: " << (ok2 ? "SUCCESS" : "FAILED") << "\n";
    inv.printFirstN(10);

    return 0;
}
