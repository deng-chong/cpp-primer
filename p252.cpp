#include<vector>
#include<string>
#include<iostream>

class Screen;

class Window_mgr {
public:
    using ScreenIdx = std::vector<Screen>::size_type;
    Window_mgr(); // default constructor is defined after the definition of class Screen
    void clear(ScreenIdx);
    void display() const;
private:
    std::vector<Screen> screens;
    //std::vector<Screen> screens{Screen(24, 80, ' ')}; // will cause a compilation error because Screen is an incomplete class.
};

class Screen {
    friend void Window_mgr::clear(ScreenIdx);
public:
    using pos = std::string::size_type;
    Screen() = default;
    Screen(pos ht, pos wd, char c): height(ht), width(wd), contents(ht * wd, c) {}
    char get() const {return contents[cur];}
    char get(pos r, pos c) const {return contents[r * width + c];}
    Screen& move(pos r, pos c) {
        cur = r * width + c;
        return *this;
    }
    Screen& set(char c) {
        contents[cur] = c;
        return *this;
    }
    Screen& set(pos r, pos c, char ch) {
        contents[r * width + c] = ch;
        return *this;
    }
    Screen& display() {
        do_display();
        return *this;
    }
    const Screen& display() const {
        do_display();
        return *this;
    }
private:
    pos cur = 0;
    pos height = 0, width = 0;
    std::string contents;

    void do_display() const {
        for (pos i = 0; i < height; ++i) {
            for (pos j = 0; j < width; ++j) {
                std::cout << contents[i * width + j];
            }
            std::cout << std::endl;
        }
    }
};

Window_mgr::Window_mgr(): screens({Screen(24, 80, ' ')}) {}

void Window_mgr::clear(ScreenIdx idx) {
    auto &s = screens[idx];
    s.contents = std::string(s.height * s.width, ' ');
    //for (char &ch:screens[idx].contents) ch = ' ';
}

int main() {

}
