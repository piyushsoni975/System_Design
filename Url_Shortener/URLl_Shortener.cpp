#include <bits/stdc++.h>
using namespace std;

/* ---------- Base62 encoder ---------- */
class Base62Encoder {
private:
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

public:
    string encode(long long id) {
        if (id == 0) return "0";

        string result;
        while (id > 0) {
            result.push_back(chars[id % 62]);
            id /= 62;
        }
        reverse(result.begin(), result.end());
        return result;
    }
};

/* ---------- Repository (Mock DB) ---------- */
class URLRepository {
private:
    unordered_map<string, string> shortToLong;
    long long autoIncrementId = 1;

public:
    long long generateId() {
        return autoIncrementId++;
    }

    void save(string shortCode, string longUrl) {
        shortToLong[shortCode] = longUrl;
    }

    string findLongUrl(string shortCode) {
        if (shortToLong.count(shortCode))
            return shortToLong[shortCode];
        return "";
    }
};

/* ---------- Service Layer ---------- */
class URLShortenerService {
private:
    URLRepository repository;
    Base62Encoder encoder;
    string domain = "https://tiny.ly/";

public:
    string shortenURL(string longUrl) {
        long long id = repository.generateId();
        string shortCode = encoder.encode(id);
        repository.save(shortCode, longUrl);
        return domain + shortCode;
    }

    string getOriginalURL(string shortUrl) {
        // extract shortCode from URL
        int pos = shortUrl.find_last_of('/');
        string shortCode = shortUrl.substr(pos + 1);
        return repository.findLongUrl(shortCode);
    }
};

/* ---------- Controller ---------- */
class URLShortenerController {
private:
    URLShortenerService service;

public:
    string createShortURL(string longUrl) {
        return service.shortenURL(longUrl);
    }

    void redirectURL(string shortUrl) {
        string originalUrl = service.getOriginalURL(shortUrl);
        if (originalUrl.empty()) {
            cout << "404 Not Found" << endl;
        } else {
            cout << "Redirecting to: " << originalUrl << endl;
        }
    }
};

/* ---------- MAIN ---------- */
int main() {
    URLShortenerController controller;

    string longUrl1 = "https://www.google.com/search?q=system+design";
    string longUrl2 = "https://leetcode.com/problems/design-tinyurl/";

    string shortUrl1 = controller.createShortURL(longUrl1);
    string shortUrl2 = controller.createShortURL(longUrl2);

    cout << "Short URL 1: " << shortUrl1 << endl;
    cout << "Short URL 2: " << shortUrl2 << endl;

    cout << endl << "Redirect Test:" << endl;
    controller.redirectURL(shortUrl1);
    controller.redirectURL(shortUrl2);
    controller.redirectURL("https://tiny.ly/invalid");

    return 0;
}
