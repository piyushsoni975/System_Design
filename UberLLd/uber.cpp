#include<bits/stdc++.h>
using namespace std;

// Point class (2D coordinates) + Manhattan distance calculation
class Point {
public:
    int x, y;
    Point(int xx=0, int yy=0): x(xx), y(yy) {}
    static int manhattan(const Point& a, const Point& b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
};


// ---------- domain ----------
enum DriverStatus { OFFLINE, ONLINE, ON_TRIP };

// Driver entity (id, name, status, location)
class Driver {
public:
    int id;
    string name;
    DriverStatus status;
    Point location;

    Driver(int id_, string name_, DriverStatus st=ONLINE, Point loc=Point())
        : id(id_), name(std::move(name_)), status(st), location(loc) {}
};

// Rider entity (id + name)
class Rider {
public:
    int id;
    string name;
    Rider(int id_, string name_): id(id_), name(std::move(name_)) {}
};

// Trip entity (stores info about one booking)
class Trip {
public:
    int id;
    int riderId;
    int driverId;
    Point pickup, drop;
    int quote;
    int finalFare;

    Trip(int id_, int rId, Point pu, Point dr, int q)
        : id(id_), riderId(rId), driverId(-1),
          pickup(pu), drop(dr), quote(q), finalFare(0) {}
};

// ---------- Strategy: Pricing ----------
class IPricingStrategy {
public:
    virtual ~IPricingStrategy() = default;
    virtual int quote(const Point& pickup, const Point& drop) const = 0;
};

// FlatPricing = fixed base + per km
class FlatPricing : public IPricingStrategy {
    int base, perUnit;
public:
    FlatPricing(int b=30, int p=12): base(b), perUnit(p) {}
    int quote(const Point& a, const Point& b) const override {
        return base + perUnit * Point::manhattan(a,b);
    }
};

// SurgePricing: flat pricing × surge multiplier
class SurgePricing : public IPricingStrategy {
    int base, perUnit, surgePct;
public:
    SurgePricing(int b=30, int p=12, int s=150): base(b), perUnit(p), surgePct(s) {}
    int quote(const Point& a, const Point& b) const override {
        int raw = base + perUnit * Point::manhattan(a,b);
        return (raw * surgePct) / 100;
    }
};

// ---------- Strategy: Matching ----------
// Interface: Driver matching ka abstraction
class IMatcher {
public:
    virtual ~IMatcher() = default;
    virtual int findDriverId(const Point& pickup, const vector<Driver>& ds) const = 0;
};

// NearestMatcher: nearest available driver find karta hai
class NearestMatcher : public IMatcher {
public:
    int findDriverId(const Point& pickup, const vector<Driver>& ds) const override {
        int bestId = -1, bestDist = 1000000000;
        for(const auto& d: ds){
            if(d.status != ONLINE) continue;
            int dist = Point::manhattan(pickup, d.location);
            if(dist < bestDist){ bestDist = dist; bestId = d.id; }
        }
        return bestId;
    }
};

class TripService {
    const IPricingStrategy& pricing;   // Strategy pattern
    const IMatcher& matcher;           // Strategy pattern
    vector<Driver>& drivers;
    vector<Trip> trips;
    int nextTripId{1};

    // helper: trip ko id se search kare
    Trip* findTrip(int tripId){
        for(auto& t: trips) if(t.id == tripId) return &t;
        return nullptr;
    }

public:
    TripService(const IPricingStrategy& p, const IMatcher& m, vector<Driver>& ds)
        : pricing(p), matcher(m), drivers(ds) {}

    // Rider trip request karta hai → fare estimate nikal ke Trip create hota hai
    int requestTrip(const Rider& r, const Point& pickup, const Point& drop){
        int q = pricing.quote(pickup, drop);
        Trip t(nextTripId++, r.id, pickup, drop, q);
        trips.push_back(t);
        return t.id;
    }

    // Driver assign karo (nearest driver strategy use karke)
    bool assignDriver(int tripId){
        Trip* t = findTrip(tripId);
        if(!t) return false;
        int did = matcher.findDriverId(t->pickup, drivers);
        if(did == -1) return false;
        t->driverId = did;
        for(auto& d: drivers) if(d.id == did) d.status = ON_TRIP;
        return true;
    }

    // Ride start karo
    bool startTrip(int tripId){
        Trip* t = findTrip(tripId);
        if(!t || t->driverId == -1) return false;
        cout << "Trip " << tripId << " started.\n";
        return true;
    }

    // Ride complete karo + final fare calculate karo
    bool completeTrip(int tripId){
        Trip* t = findTrip(tripId);
        if(!t || t->driverId == -1) return false;
        t->finalFare = pricing.quote(t->pickup, t->drop);
        for(auto& d: drivers) if(d.id == t->driverId){
            d.status = ONLINE;
            d.location = t->drop;
        }
        cout << "Trip " << tripId << " completed. Fare = " << t->finalFare << "\n";
        return true;
    }

    // Trip ka pointer return kare (read-only)
    const Trip* getTrip(int tripId) const {
        for(const auto& t: trips) if(t.id == tripId) return &t;
        return nullptr;
    }
};

// ---------- Demo ----------
int main(){
    // Seed drivers
    vector<Driver> drivers = {
        Driver(1,"Amit",ONLINE, Point(0,0)),
        Driver(2,"Beena",ONLINE, Point(5,2)),
        Driver(3,"Chirag",OFFLINE,Point(9,9))
    };
    Rider r(101,"Ravi");

    // Strategy objects
    FlatPricing flat;      // Pricing
    NearestMatcher nearest; // Matching

    // TripService uses abstraction (DIP)
    TripService service(flat, nearest, drivers);

    // Step1: request trip
    int tripId = service.requestTrip(r, Point(1,1), Point(6,4));
    cout << "Trip " << tripId << " requested. Quote = "
         << service.getTrip(tripId)->quote << "\n";

    // Step2: assign driver
    if(service.assignDriver(tripId)){
        cout << "Driver assigned.\n";
    } else {
        cout << "No driver available.\n"; return 0;
    }

    // Step3: start and complete ride
    service.startTrip(tripId);
    service.completeTrip(tripId);

    return 0;
}
