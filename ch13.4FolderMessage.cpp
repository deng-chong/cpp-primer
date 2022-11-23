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
    void addMessage(Message* m); // add Message `m` to `messages`
    void remMessage(Message* m); // remove Message `m` from `messages`
public:
    Folder() = default;
    ~Folder();
    Folder(const Folder&);
    Folder(Folder&&);
    Folder& operator=(const Folder&);
    Folder& operator=(Folder&&);

    void show() const;
};

class Message {
    friend void swap(Message& lhs, Message& rhs);
    friend class Folder;
private:
    std::string content;
    std::unordered_set<Folder*> folders;

    void addFolder(Folder* f) { // add Folder `f` to `folders`
        if (!folders.count(f)) {
            folders.insert(f);
        }
    }

    void remFolder(Folder* f) { // remove Folder `f` from `folders`
        if (folders.count(f)) {
            folders.erase(f);
        }
    }
public:
    Message(const std::string& s=""): content(s) {}

    ~Message() {
        for (auto f:folders) f->remMessage(this);
    }

    Message(const Message& rhs): content(rhs.content), folders(rhs.folders) {
        //std::cout << "Message(const Message&)\n";
        for (auto f:folders) f->addMessage(this);
    }

    Message(Message&& rhs): content(std::move(rhs.content)), folders(std::move(rhs.folders)) {
        //std::cout << "Message(Message&&)\n";
        rhs.content = std::string(); //置于析构安全状态
        rhs.folders = std::unordered_set<Folder*>(); //置于析构安全状态
        for (auto f:folders) {
            f->remMessage(&rhs); //rhs将要销毁，要断开关联
            f->addMessage(this);
        }
    }

    Message& operator=(const Message& rhs) {
        //std::cout << "operator=(const Message&)\n";
        if (this != &rhs) {
            for (auto f:folders) f->remMessage(this);
            content = rhs.content;
            folders = rhs.folders;
            for (auto f:folders) f->addMessage(this);
        }
        return *this;
    }

    Message& operator=(Message&& rhs) {
        //std::cout << "operator=(Message&&)\n";
        if (this != &rhs) {
            for (auto f:folders) f->remMessage(this);
            content = std::move(rhs.content);
            folders = std::move(rhs.folders);
            rhs.content = std::string(); //置于析构安全状态
            rhs.folders = std::unordered_set<Folder*>(); //置于析构安全状态
            for (auto f:folders) {
                f->remMessage(&rhs); //rhs将要销毁，要断开关联
                f->addMessage(this);
            }
        }
        return *this;
    }

    void save(Folder& f) { // save current message to Folder `f`
        f.addMessage(this);
        addFolder(&f);
    }

    void remove(Folder& f) { // remove current message from Folder `f`
        f.remMessage(this);
        remFolder(&f);
    }
};

Folder::~Folder() {
    for (auto m:messages) m->remFolder(this);
}

Folder::Folder(const Folder& rhs) {
    //std::cout << "Folder(const Folder&)\n";
    messages = rhs.messages;
    for (auto m:messages) m->addFolder(this);
}

Folder::Folder(Folder&& rhs) {
    //std::cout << "Folder(Folder&&)\n";
    messages = std::move(rhs.messages);
    rhs.messages = std::unordered_set<Message*>(); //置于析构安全状态
    for (auto m:messages) {
        m->remFolder(&rhs); //rhs将要销毁，要断开关联
        m->addFolder(this);
    }
}  

Folder& Folder::operator=(const Folder& rhs) {
    //std::cout << "operator=(const Folder&)\n";
    if (this != &rhs) {
        for (auto m:messages) m->remFolder(this);
        messages = rhs.messages;
        for (auto m:messages) m->addFolder(this);
    }
    return *this;
}

Folder& Folder::operator=(Folder&& rhs) {
    //std::cout << "operator=(Folder&&)\n";
    if (this != &rhs) {
        for (auto m:messages) m->remFolder(this);
        messages = std::move(rhs.messages);
        rhs.messages = std::unordered_set<Message*>();//置于析构安全状态
        for (auto m:messages) {
            m->remFolder(&rhs);//rhs将要销毁，要断开关联
            m->addFolder(this);
        }
    }
    return *this;
}

void Folder::show() const {
    for (auto m:messages) std::cout << m->content << " ";
}

void Folder::addMessage(Message* m) {
    if (!messages.count(m)) {
        messages.insert(m);
    }
}

void Folder::remMessage(Message* m) {
    if (messages.count(m)) {
        messages.erase(m);
    }
}

void swap(Message& lhs, Message& rhs) {
    for (auto f:lhs.folders) f->remMessage(&lhs);
    for (auto f:rhs.folders) f->remMessage(&rhs);
    std::swap(lhs.content, rhs.content);
    std::swap(lhs.folders, rhs.folders);
    for (auto f:lhs.folders) f->addMessage(&lhs);
    for (auto f:rhs.folders) f->addMessage(&rhs);
}


int main() {
    Folder A, B, C(A);
    Message x("x"), y("y"), z("z");
    x.save(A);
    y.save(A);
    y.save(B);
    z.save(C);
    z.save(A);
    Folder f(std::move(B));
    Message m(std::move(x));
    f = std::move(f);
    m = std::move(z);
    std::cout << "A:\t";
    A.show();
    std::cout << "\nB:\t";
    B.show();
    std::cout << "\nC:\t";
    C.show();
    std::cout << "\nf:\t";
    f.show();
}
