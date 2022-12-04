#include<iostream>
#include<string>
using std::cout;

class Quote {
private:
    std::string bookNo;
protected:
    double price = 0; // initial price
public:
    Quote() = default;
    Quote(const std::string& book): bookNo(book) {}
    Quote(const std::string& book, double p): bookNo(book), price(p) {}
    Quote(const Quote&) = default;
    Quote& operator=(const Quote&) = default;
    virtual ~Quote() = default;
    virtual double net_price(std::size_t n) const { return n * price; }
    std::string isbn() const { return bookNo; }
};

class Disc_quote : public Quote {
protected:
    std::size_t quantity = 0;
    double discount = 0;
public:
    Disc_quote() = default;
    Disc_quote(const std::string& book, double p, std::size_t qty, double disc)
    : Quote(book, p), quantity(qty), discount(disc) {}
    Disc_quote(const Quote& q, std::size_t qty, double disc)
    : Quote(q), quantity(qty), discount(disc) {}
    double net_price(std::size_t) const override = 0;
};

class Bulk_quote : public Disc_quote {
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string& book, double p, std::size_t qty, double disc)
    : Disc_quote(book, p, qty, disc) {}
    Bulk_quote(const Quote& q, std::size_t qty, double disc)
    : Disc_quote(q, qty, disc) {}
    double net_price(std::size_t n) const override { return n >= quantity ? (1 - discount) * n * price : n * price; }
};

class Limit_quote : public Disc_quote {
public:
    Limit_quote() = default;
    Limit_quote(const std::string& book, double p, std::size_t qty, double disc)
    : Disc_quote(book, p, qty, disc) {}
    Limit_quote(const Quote& q, std::size_t qty, double disc)
    : Disc_quote(q, qty, disc) {}
    double net_price(std::size_t n) const override { 
        return n <= quantity ? n * price * (1 - discount) : (quantity * (1 - discount) + n - quantity) * price; 
    }
};

int main() {
    
}
