#include<iostream>
#include<string>
#include<set>
#include<unordered_set>

class Message;

class Folder {
    friend void swap(Message& lhs, Message& rhs);
    friend class Message;
private:
    std::unordered_set<Message*> messages;
    void addMessage(Message& m); // add Message `m` to `messages`
    void remMessage(Message& m); // remove Message `m` from `messages`
public:
    Folder() = default;
    ~Folder();
    Folder(const Folder&);
    Folder& operator=(const Folder&);
};

class Message {
    friend void swap(Message& lhs, Message& rhs);
    friend class Folder;
private:
    std::string content;
    std::unordered_set<Folder*> folders;

    void addFolder(Folder& f) { // add Folder `f` to `folders`
        if (!folders.count(&f)) {
            folders.insert(&f);
        }
    }

    void remFolder(Folder& f) { // remove Folder `f` from `folders`
        if (folders.count(&f)) {
            folders.erase(&f);
        }
    }
public:
    Message(const std::string& s=""): content(s) {}

    ~Message() {
        for (auto f:folders) f->remMessage(*this);
    }

    Message(const Message& rhs): content(rhs.content), folders(rhs.folders) {
        for (auto f:folders) f->addMessage(*this);
    }

    Message& operator=(const Message& rhs) {
        if (this != &rhs) {
            for (auto f:folders) f->remMessage(*this);
            content = rhs.content;
            folders = rhs.folders;
            for (auto f:folders) f->addMessage(*this);
        }
        return *this;
    }

    void save(Folder& f) { // save current message to Folder `f`
        f.addMessage(*this);
        addFolder(f);
    }

    void remove(Folder& f) { // remove current message from Folder `f`
        f.remMessage(*this);
        remFolder(f);
    }
};

Folder::~Folder() {
    for (auto m:messages) m->remFolder(*this);
}

Folder::Folder(const Folder& rhs) {
    messages = rhs.messages;
    for (auto m:messages) m->addFolder(*this);
}

Folder& Folder::operator=(const Folder& rhs) {
    if (this != &rhs) {
        for (auto m:messages) m->remFolder(*this);
        messages = rhs.messages;
        for (auto m:messages) m->addFolder(*this);
    }
    return *this;
}

void Folder::addMessage(Message& m) {
    if (!messages.count(&m)) {
        messages.insert(&m);
    }
}

void Folder::remMessage(Message& m) {
    if (messages.count(&m)) {
        messages.erase(&m);
    }
}

void swap(Message& lhs, Message& rhs) {
    for (auto f:lhs.folders) f->remMessage(lhs);
    for (auto f:rhs.folders) f->remMessage(rhs);
    std::swap(lhs.content, rhs.content);
    std::swap(lhs.folders, rhs.folders);
    for (auto f:lhs.folders) f->addMessage(lhs);
    for (auto f:rhs.folders) f->addMessage(rhs);
}

int main() {
    Folder A, B;
    Message x("Alice"), y("Bob"), z;
    x.save(A);
    x.save(B);
    y.save(A);
    B = A;
    z = x;
    Folder f(A);
    Message m(x);
    x.remove(A);
    //std::cout << "The end.\n";
    return 0;
}
