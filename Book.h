#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <memory>

class Book {
private:
    std::string title;
    std::string isbn;
    std::string author;
    std::string publisher;
    double price;
    std::string publishDate;
    int stock;
    int borrowCount;

public:
    Book(const std::string& title, const std::string& isbn, const std::string& author,
         const std::string& publisher, double price, const std::string& publishDate,
         int stock, int borrowCount = 0);
    
    std::string getTitle() const { return title; }
    std::string getIsbn() const { return isbn; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    double getPrice() const { return price; }
    std::string getPublishDate() const { return publishDate; }
    int getStock() const { return stock; }
    int getBorrowCount() const { return borrowCount; }
    
    void setTitle(const std::string& title) { this->title = title; }
    void setIsbn(const std::string& isbn) { this->isbn = isbn; }
    void setAuthor(const std::string& author) { this->author = author; }
    void setPublisher(const std::string& publisher) { this->publisher = publisher; }
    void setPrice(double price) { this->price = price; }
    void setPublishDate(const std::string& publishDate) { this->publishDate = publishDate; }
    void setStock(int stock) { this->stock = stock; }
    void setBorrowCount(int count) { this->borrowCount = count; }
    
    void incrementBorrowCount() { borrowCount++; }
    void decrementStock() { if (stock > 0) stock--; }
    void incrementStock() { stock++; }
    
    std::string toString() const;
    static std::shared_ptr<Book> fromString(const std::string& str);
};

#endif

