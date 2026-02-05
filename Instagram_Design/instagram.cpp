#include <bits/stdc++.h>
using namespace std;

// ------------------- MODELS -------------------

class User {
public:
    int userId;
    string username;
    string email;

    unordered_set<int> followers;
    unordered_set<int> following;

    User(int id, string name, string mail) {
        userId = id;
        username = name;
        email = mail;
    }
};

class Post {
public:
    int postId;
    int userId;
    string imageUrl;
    string caption;
    long timestamp;

    unordered_set<int> likes;
    vector<int> comments;

    Post(int pid, int uid, string img, string cap) {
        postId = pid;
        userId = uid;
        imageUrl = img;
        caption = cap;
        timestamp = time(0);
    }
};

class Comment {
public:
    int commentId;
    int postId;
    int userId;
    string text;
    long timestamp;

    Comment(int cid, int pid, int uid, string t) {
        commentId = cid;
        postId = pid;
        userId = uid;
        text = t;
        timestamp = time(0);
    }
};

// ------------------- SERVICES -------------------

class UserService {
    unordered_map<int, User*> users;
    int userCounter = 1;

public:
    User* createUser(string name, string email) {
        User* u = new User(userCounter++, name, email);
        users[u->userId] = u;
        return u;
    }

    User* getUser(int id) {
        return users[id];
    }
};

class FollowService {
public:
    void follow(User* u1, User* u2) {
        u1->following.insert(u2->userId);
        u2->followers.insert(u1->userId);
    }

    void unfollow(User* u1, User* u2) {
        u1->following.erase(u2->userId);
        u2->followers.erase(u1->userId);
    }
};

class PostService {
public:
    unordered_map<int, Post*> posts;
    int postCounter = 1;

    Post* createPost(int userId, string img, string caption) {
        Post* p = new Post(postCounter++, userId, img, caption);
        posts[p->postId] = p;
        return p;
    }

    Post* getPost(int id) {
        return posts[id];
    }
};

class LikeService {
public:
    void likePost(Post* post, int userId) {
        post->likes.insert(userId);
    }

    void unlikePost(Post* post, int userId) {
        post->likes.erase(userId);
    }
};

class CommentService {
    unordered_map<int, Comment*> comments;
    int commentCounter = 1;

public:
    Comment* addComment(Post* post, int userId, string text) {
        Comment* c = new Comment(commentCounter++, post->postId, userId, text);
        comments[c->commentId] = c;
        post->comments.push_back(c->commentId);
        return c;
    }
};

// ------------------- FEED SERVICE -------------------

class FeedService {
    PostService* postService;
    UserService* userService;

public:
    FeedService(PostService* ps, UserService* us) {
        postService = ps;
        userService = us;
    }

    vector<Post*> getFeed(User* user) {
        vector<Post*> feed;

        for(auto followeeId : user->following) {
            for(auto &p : postService->posts) {
                if(p.second->userId == followeeId)
                    feed.push_back(p.second);
            }
        }

        sort(feed.begin(), feed.end(),
            [](Post* a, Post* b) {
                return a->timestamp > b->timestamp;
            });

        return feed;
    }
};

// ------------------- MAIN DRIVER PROGRAM -------------------

int main() {

    cout << "---- Instagram LLD Simulation ----\n\n";

    // Initialize Services
    UserService userService;
    PostService postService;
    FollowService followService;
    LikeService likeService;
    CommentService commentService;
    FeedService feedService(&postService, &userService);

    // Create Users
    User* alice = userService.createUser("Alice", "alice@gmail.com");
    User* bob = userService.createUser("Bob", "bob@gmail.com");
    User* charlie = userService.createUser("Charlie", "charlie@gmail.com");

    cout << "Users Created:\n";
    cout << alice->username << ", " << bob->username << ", " << charlie->username << "\n\n";

    // Follow Operations
    followService.follow(alice, bob);
    followService.follow(alice, charlie);

    cout << "Alice followed Bob and Charlie\n\n";

    // Create Posts
    Post* p1 = postService.createPost(bob->userId, "bob_pic.jpg", "Hello from Bob!");
    Post* p2 = postService.createPost(charlie->userId, "charlie_pic.jpg", "Good Morning!");

    cout << "Bob and Charlie uploaded posts\n\n";

    // Like Posts
    likeService.likePost(p1, alice->userId);
    likeService.likePost(p2, alice->userId);

    cout << "Alice liked both posts\n\n";

    // Comment on Post
    commentService.addComment(p1, alice->userId, "Nice post Bob!");

    cout << "Alice commented on Bob's post\n\n";

    // Fetch Feed for Alice
    vector<Post*> feed = feedService.getFeed(alice);

    cout << "---- Alice's Feed ----\n";

    for(Post* post : feed) {
        cout << "Post ID: " << post->postId << "\n";
        cout << "User ID: " << post->userId << "\n";
        cout << "Caption: " << post->caption << "\n";
        cout << "Likes: " << post->likes.size() << "\n";
        cout << "Comments: " << post->comments.size() << "\n";
        cout << "----------------------\n";
    }

    return 0;
}
