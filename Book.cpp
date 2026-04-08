#include "Book.h"
#include <sstream>

Book::Book(const std::string& title, const std::string& isbn, const std::string& author,
           const std::string& publisher, double price, const std::string& publishDate,
           int stock, int borrowCount)
    : title(title), isbn(isbn), author(author), publisher(publisher),
      price(price), publishDate(publishDate), stock(stock), borrowCount(borrowCount) {}

std::string Book::toString() const { 
    std::ostringstream oss;
    oss << title << "," << isbn << "," << author << "," 
        << publisher << "," << price << "," << publishDate << "," 
        << stock << "," << borrowCount;
    return oss.str();
}

std::shared_ptr<Book> Book::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string title, isbn, author, publisher, publishDate;
    double price;
    int stock, borrowCount;
    
    std::getline(iss, title, ',');
    std::getline(iss, isbn, ',');
    std::getline(iss, author, ',');
    std::getline(iss, publisher, ',');
    iss >> price;
    iss.ignore();
    std::getline(iss, publishDate, ',');
    iss >> stock;
    iss.ignore();
    iss >> borrowCount;
    
    return std::make_shared<Book>(title, isbn, author, publisher, price, publishDate, stock, borrowCount);
}

