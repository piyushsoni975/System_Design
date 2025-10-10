#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// ---------------- Vehicle ----------------
class Vehicle {
    string plate;
    string size; // small, medium, large
public:
    Vehicle(string p, string s) : plate(p), size(s) {}
    string getPlate() { return plate; }
    string getSize() { return size; }
};

// ---------------- Spot ----------------
class Spot {
    int id;
    string size;
    bool occupied;
public:
    Spot(int i, string s) : id(i), size(s), occupied(false) {}
    bool canFit(Vehicle v) { return !occupied && v.getSize() == size; }
    void occupy() { occupied = true; }
    void free() { occupied = false; }
    int getId() { return id; }
    string getSize() { return size; }
    bool isOccupied() { return occupied; }
};

// ---------------- Ticket ----------------
class Ticket {
    string id;
    string plate;
    int spotId;
    int hours; // parking duration
public:
    Ticket() {} // <--- Default constructor for VC++
    Ticket(string tid, string p, int sid, int h) : id(tid), plate(p), spotId(sid), hours(h) {}
    string getId() { return id; }
    string getPlate() { return plate; }
    int getSpotId() { return spotId; }
    int getHours() { return hours; }
};

// ---------------- Pricing Strategy ----------------
class PricingStrategy {
public:
    virtual double calculate(int hours) = 0;
    virtual ~PricingStrategy() {}
};

class HourlyPricing : public PricingStrategy {
    double rate;
public:
    HourlyPricing(double r) : rate(r) {}
    double calculate(int hours) override { return (hours <= 0 ? 1 : hours) * rate; }
};

// ---------------- Parking Lot ----------------
class ParkingLot {
    vector<Spot> spots;
    map<string, Ticket> tickets;
    PricingStrategy* pricing;
    int ticketSeq;
public:
    ParkingLot(PricingStrategy* ps) : pricing(ps), ticketSeq(1) {
        spots.push_back(Spot(1, "small"));
        spots.push_back(Spot(2, "medium"));
        spots.push_back(Spot(3, "large"));
    }

    string park(Vehicle v, int hours) {
        for (auto &s : spots) {
            if (s.canFit(v)) {
                s.occupy();
                string tid = "T" + to_string(ticketSeq++);
                tickets[tid] = Ticket(tid, v.getPlate(), s.getId(), hours);
                cout << v.getPlate() << " parked at Spot " << s.getId() 
                     << " with Ticket " << tid << endl;
                return tid;
            }
        }
        cout << "No spot available for " << v.getPlate() << endl;
        return "";
    }

    void leave(string tid) {
        if (tickets.find(tid) == tickets.end()) {
            cout << "Invalid ticket\n";
            return;
        }
        Ticket t = tickets[tid];
        for (auto &s : spots) {
            if (s.getId() == t.getSpotId() && s.isOccupied()) {
                s.free();
                double fee = pricing->calculate(t.getHours());
                cout << t.getPlate() << " left Spot " << s.getId() 
                     << ". Fee: " << fee << endl;
                break;
            }
        }
        tickets.erase(tid);
    }
};

// ---------------- Main ----------------
int main() {
    PricingStrategy* pricing = new HourlyPricing(20); // 20 per hour
    ParkingLot lot(pricing);

    Vehicle v1("CAR123", "medium");
    Vehicle v2("BIKE7", "small");
    Vehicle v3("TRUCK9", "large");

    string t1 = lot.park(v1, 3); // 3 hours
    string t2 = lot.park(v2, 1); // 1 hour
    string t3 = lot.park(v3, 5); // 5 hours

    lot.leave(t1);
    lot.leave(t2);
    lot.leave(t3);

    delete pricing;
    return 0;
}
 