#include <bits/stdc++.h>
using namespace std;

class Point {
public:
    int x, y;
    Point(int xx = 0, int yy = 0) {
        x = xx;
        y = yy;
    }

    static int manhattan(const Point &a, const Point &b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
};

enum Driverstatus { OFFLINE, ONLINE, ON_TRIP };

class Driver {
public:
    int id;
    string name;
    Driverstatus status;
    Point location;

    Driver(int id_, string name_, Driverstatus st = ONLINE, Point loc = Point()) {
        id = id_;
        name = name_;
        status = st;
        location = loc;
    }
};

class Rider {
public:
    int id;
    string name;
    Rider(int id_, string name_) {
        id = id_;
        name = name_;
    }
};

class Trip {
public:
    int id;
    int riderid;
    int driverid;
    Point pickup, drop;
    int quote;
    int finalfare;

    Trip(int id_, int rid, Point pi, Point dr, int q) {
        id = id_;
        riderid = rid;
        driverid = -1;
        pickup = pi;
        drop = dr;
        quote = q;
        finalfare = 0;
    }
};

class Pricingstrategy {
public:
    virtual int quote(const Point &pickup, const Point &drop) = 0;
};

class Flatprice : public Pricingstrategy {
    int base, perunit;

public:
    Flatprice(int b = 30, int p = 12) {
        base = b;
        perunit = p;
    }

    int quote(const Point &a,const  Point &b) override {
        return base + perunit * Point::manhattan(a, b);
    }
};

class Surgeprice : public Pricingstrategy {
    int base, perunit, surge;

public:
    Surgeprice(int b = 30, int p = 12, int s = 150) {
        base = b;
        perunit = p;
        surge = s;
    }

    int quote(const Point &a, const Point &b) override {
        int raw = base + perunit * Point::manhattan(a, b);
        return (raw * surge) / 100;
    }
};

class Matcher {
public:
    virtual int finddriver(Point &pickup, vector<Driver> &dri) = 0;
};

class Nearestmatcher : public Matcher {
public:
    int finddriver(Point &pickup, vector<Driver> &dri) override {
        int bestid = -1;
        int bestdis = 1e9;
        for (auto &it : dri) {
            if (it.status != ONLINE)
                continue;
            int dis = Point::manhattan(pickup, it.location);
            if (dis < bestdis) {
                bestdis = dis;
                bestid = it.id;
            }
        }
        return bestid;
    }
};

class Tripservice {
    Pricingstrategy &price;
    Matcher &matcher;
    vector<Driver> &drivers;
    vector<Trip> trips;
    int nexttripid = 1; // start from 1

    Trip *findtrip(int tripid) {
        for (auto &it : trips) {
            if (it.id == tripid) {
                return &it;
            }
        }
        return NULL;
    }

public:
    Tripservice(Pricingstrategy &p, Matcher &m, vector<Driver> &ds)
        : price(p), matcher(m), drivers(ds) {}

    int requesttrip(Rider &r, Point &pickup, Point &drop) {
        int q = price.quote(pickup, drop);
        Trip t(nexttripid++, r.id, pickup, drop, q);
        trips.push_back(t);
        return t.id;
    }

    bool assigndriver(int tripid) {
        Trip *t = findtrip(tripid);
        if (!t) {
            return false;
        }
        int did = matcher.finddriver(t->pickup, drivers);
        if (did == -1) {
            return false;
        }
        t->driverid = did;
        for (auto &it : drivers) {
            if (it.id == did) {
                it.status = ON_TRIP;
            }
        }
        return true;
    }

    bool starttrip(int tripId) {
        Trip *t = findtrip(tripId);
        if (!t || t->driverid == -1)
            return false;
        cout << "Trip " << tripId << " started.\n";
        return true;
    }

    bool completetrip(int tripid) {
        Trip *t = findtrip(tripid);
        if (!t || t->driverid == -1) {
            return false;
        }
        t->finalfare = price.quote(t->pickup, t->drop); // FIXED (was == before)
        for (auto &it : drivers) {
            if (it.id == t->driverid) {
                it.status = ONLINE;
                it.location = t->drop;
            }
        }
        cout << "Trip " << tripid << " completed. Fare = " << t->finalfare << endl;
        return true;
    }

    Trip *gettrip(int tripid) {
        return findtrip(tripid);
    }
};

int main() {
    vector<Driver> drivers = {
        Driver(1, "Amit", ONLINE, Point(0, 0)),
        Driver(2, "Beena", ONLINE, Point(5, 2)),
        Driver(3, "Chirag", OFFLINE, Point(9, 9))};

    Rider r(101, "Piyush");

    Flatprice flat;
    Nearestmatcher nearest;

    Tripservice service(flat, nearest, drivers);

    
    int tripid = service.requesttrip(r, Point(1, 1), Point(6, 4));

    cout << "Trip " << tripid << " requested. Quote = " << service.gettrip(tripid)->quote << endl;

    if (service.assigndriver(tripid)) {
        cout << "Driver assigned.\n";
    } else {
        cout << "No driver available.\n";
        return 0;
    }

    service.starttrip(tripid);
    service.completetrip(tripid);

    return 0;
}
