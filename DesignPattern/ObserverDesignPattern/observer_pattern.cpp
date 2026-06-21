#include <iostream>
#include <vector>
using namespace std;

class Observer {
public:
    virtual void update(string msg) = 0;
};

class Subscriber : public Observer {
    string name;

public:
    Subscriber(string name) : name(name) {}

    void update(string msg) override {
        cout << name << " received: " << msg << endl;
    }
};

class YouTubeChannel {
    vector<Observer*> subscribers;

public:
    void subscribe(Observer* obs) {
        subscribers.push_back(obs);
    }

    void notify(string msg) {
        for (auto sub : subscribers) {
            sub->update(msg);
        }
    }

    void uploadVideo(string title) {
        cout << "Uploaded: " << title << endl;
        notify("New Video: " + title);
    }
};

int main() {
    YouTubeChannel channel;

    Subscriber s1("Piyush");
    Subscriber s2("Rahul");

    channel.subscribe(&s1);
    channel.subscribe(&s2);

    channel.uploadVideo("Observer Pattern");
}